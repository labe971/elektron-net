# Elektron Net Faucet

A lightweight web faucet for Elektron Net (Bitcoin-based fork). Users enter their wallet address and receive a small payout directly from a hot wallet node.

## Features

- JSON-RPC integration with `elektron-net` daemon
- SQLite-backed claim tracking (no external DB needed)
- Rate limiting per IP + per wallet address
- Daily global payout cap (anti-drain)
- Simple, mobile-friendly UI

## Prerequisites

1. A running **Elektron Net node** with RPC enabled.
2. A funded **wallet** on that node (use a small hot-wallet amount).
3. Node.js 18+ installed.

## RPC Configuration

In your `elektron-net.conf` (or the equivalent for this fork), ensure:

```
server=1
rpcuser=your_rpc_username
rpcpassword=your_rpc_password
rpcport=8332
rpcallowip=127.0.0.1
```

Restart the daemon after changing config.

## Install & Run

```bash
cd contrib/faucet
cp .env.example .env
# Edit .env and set your RPC credentials
npm install
npm start
```

Then open `http://localhost:3000` in your browser.

## Environment Variables

| Variable | Default | Description |
|----------|---------|-------------|
| `RPC_URL` | `http://127.0.0.1:8332` | Node RPC endpoint |
| `RPC_USER` | - | RPC username |
| `RPC_PASS` | - | RPC password |
| `FAUCET_PAYOUT` | `0.1` | Amount sent per claim |
| `COOLDOWN_HOURS` | `24` | Hours between claims |
| `DAILY_LIMIT` | `1000` | Max EL sent per 24h |
| `PORT` | `3000` | Web server port |

## Security Notes

- Only load a small amount into the wallet used by the faucet.
- Use a firewall to restrict RPC to localhost.
- For a public deployment, add a CAPTCHA (e.g. hCaptcha / Cloudflare Turnstile) in front of `/api/claim`.
- Consider running behind a reverse proxy (nginx, Caddy) with HTTPS.
