const express = require('express');
const axios = require('axios');
const fs = require('fs');
const path = require('path');
const rateLimit = require('express-rate-limit');
require('dotenv').config();

const app = express();
const PORT = process.env.PORT || 3000;

const RPC_URL = process.env.RPC_URL || 'http://127.0.0.1:8332';
const RPC_USER = process.env.RPC_USER || '';
const RPC_PASS = process.env.RPC_PASS || '';
const FAUCET_PAYOUT = parseFloat(process.env.FAUCET_PAYOUT || '0.1');
const COOLDOWN_HOURS = parseInt(process.env.COOLDOWN_HOURS || '24', 10);
const DAILY_LIMIT = parseFloat(process.env.DAILY_LIMIT || '1000');

app.use(express.json());
app.use(express.static('public'));

const limiter = rateLimit({
  windowMs: 15 * 60 * 1000,
  max: 30,
  standardHeaders: true,
  legacyHeaders: false,
});
app.use(limiter);

// Simple JSON-file persistence (no native deps)
const DB_FILE = path.join(__dirname, 'claims.json');
function loadDb() {
  if (!fs.existsSync(DB_FILE)) return { claims: [] };
  try {
    return JSON.parse(fs.readFileSync(DB_FILE, 'utf8'));
  } catch {
    return { claims: [] };
  }
}
function saveDb(db) {
  fs.writeFileSync(DB_FILE, JSON.stringify(db, null, 2));
}

function lastClaimByAddress(address) {
  const db = loadDb();
  const found = db.claims.filter(c => c.address === address);
  if (!found.length) return null;
  return found.sort((a, b) => b.claimed_at - a.claimed_at)[0];
}

function lastClaimByIp(ip) {
  const db = loadDb();
  const found = db.claims.filter(c => c.ip === ip);
  if (!found.length) return null;
  return found.sort((a, b) => b.claimed_at - a.claimed_at)[0];
}

function dailyTotal() {
  const db = loadDb();
  const dayAgo = Date.now() - 24 * 60 * 60 * 1000;
  return db.claims
    .filter(c => c.claimed_at > dayAgo)
    .reduce((sum, c) => sum + c.amount, 0);
}

function addClaim(address, ip, amount, txid) {
  const db = loadDb();
  db.claims.push({ address, ip, amount, txid, claimed_at: Date.now() });
  saveDb(db);
}

// RPC helper
async function rpcCall(method, params = []) {
  const res = await axios.post(
    RPC_URL,
    { jsonrpc: '1.0', id: 'faucet', method, params },
    {
      auth: { username: RPC_USER, password: RPC_PASS },
      headers: { 'Content-Type': 'application/json' },
      timeout: 30000,
    }
  );
  if (res.data.error) throw new Error(res.data.error.message);
  return res.data.result;
}

// Validate basic Elektron/Bitcoin address shape
function isValidAddress(addr) {
  if (!addr || typeof addr !== 'string') return false;
  return /^(1|3|bc1)[a-zA-Z0-9]{25,62}$/.test(addr) || /^(E|e)[a-zA-Z0-9]{25,62}$/.test(addr);
}

// Claim endpoint
app.post('/api/claim', async (req, res) => {
  const { address } = req.body;
  const ip = req.headers['x-forwarded-for'] || req.socket.remoteAddress || 'unknown';

  if (!isValidAddress(address)) {
    return res.status(400).json({ error: 'Invalid wallet address.' });
  }

  const now = Date.now();
  const cooldownMs = COOLDOWN_HOURS * 60 * 60 * 1000;

  const addrRow = lastClaimByAddress(address);
  if (addrRow && now - addrRow.claimed_at < cooldownMs) {
    const wait = Math.ceil((addrRow.claimed_at + cooldownMs - now) / (1000 * 60));
    return res.status(429).json({ error: `Address already claimed. Wait ${wait} minutes.` });
  }

  const ipRow = lastClaimByIp(ip);
  if (ipRow && now - ipRow.claimed_at < cooldownMs) {
    const wait = Math.ceil((ipRow.claimed_at + cooldownMs - now) / (1000 * 60));
    return res.status(429).json({ error: `IP already claimed. Wait ${wait} minutes.` });
  }

  const today = dailyTotal();
  if (today >= DAILY_LIMIT) {
    return res.status(429).json({ error: 'Daily faucet limit reached. Try again tomorrow.' });
  }

  try {
    const txid = await rpcCall('sendtoaddress', [address, FAUCET_PAYOUT]);
    addClaim(address, ip, FAUCET_PAYOUT, txid);
    return res.json({ success: true, txid, amount: FAUCET_PAYOUT });
  } catch (err) {
    console.error('RPC error:', err.message);
    return res.status(500).json({ error: 'Wallet transaction failed. Check server logs.' });
  }
});

// Stats endpoint
app.get('/api/stats', (req, res) => {
  const today = dailyTotal();
  res.json({ dailyLimit: DAILY_LIMIT, dailyUsed: today, payout: FAUCET_PAYOUT, cooldownHours: COOLDOWN_HOURS });
});

app.listen(PORT, () => {
  console.log(`Elektron Net Faucet running on http://localhost:${PORT}`);
});
