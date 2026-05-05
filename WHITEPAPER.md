# Elektron Net – Whitepaper
**Version 2.2 | Author: Ali Kutlusoy | Graz, Austria, 2026**

*"From the Lydian Elektron to the Digital Elek – Value Through Mathematics"*

---

## Prologue: On Freedom and Responsibility

*A word from the author*

This protocol was not created by an institution, a corporation, or a committee. I write this as a single human being who holds, without reservation, that every person is born free – and that this freedom must be actively defended in a digital world that was not built with freedom in mind.

Marcus Aurelius wrote: *"You have power over your mind – not outside events. Realise this, and you will find strength."* The Stoics understood something that most modern systems forget: freedom is not granted from above. It is exercised from within. It is a discipline, not a gift.

Elektron Net is my contribution to that discipline. It is a tool built from mathematics and code, inspired by 2,600 years of human history – from the Lydian merchants who traded without a king's permission to the cryptographers who proved that two strangers could share a secret without ever meeting. It is a tool. Nothing more, and nothing less.

**On authorship and control:**
I lit the first spark, but I do not own the fire. By publishing this protocol under the MIT licence, I relinquish all control. There are no administrators, no backdoors, no central authority – including me.

**On the Stoic nature of this system:**
The Stoics distinguished between what is *up to us* and what is *not up to us*. Elektron Net is designed around this distinction. What is up to the mathematics: your balance belongs to you, your payments are anonymous, your history is forgotten after 137 days. What is not up to us: market prices, adoption rates, the decisions of others. We build for what we can control.

**On responsibility:**
Freedom without responsibility is noise. If this tool gives you the power to act without surveillance, use that power to build something you would be proud of in full light.

*"Waste no more time arguing about what a good man should be. Be one."*
— Marcus Aurelius

---

*Ali Kutlusoy*
*Graz, Austria, 2026*
*https://www.elektron-net.org*

---

# Part I – Why

# 1 – Vision, Philosophy & Historical Origin

## The Lydian Elektron

Around 600 BC, the Lydians in Asia Minor struck the first coins in the world. They were made of Elektron – a naturally occurring alloy of gold and silver. These coins fundamentally changed civilisation: for the first time, a person could trade without giving their name. Value became transferable, divisible, anonymous.

The Lydian Elektron coins were the first decentralised currency in history. No king had to be present. No scribe had to record the exchange. The value lay in the material itself – in the mathematics of the alloy.

2,600 years later, the world faces the same problem, solved with the same means: mathematics instead of trust.

## 1.1 – The Pocket Philosophy

Elektron Net is deliberately built around a simple, ancient human experience: When you put on your pants in the morning and reach into your pocket, you immediately know three things:

- **How much money you have right now** — not last week, not last month.
- **That this money is yours** — because it is your pocket.
- **You do not need to remember where each coin came from**, what you bought yesterday, or who paid you last year.

*The past is irrelevant. Only the present matters.*

This everyday reality is the guiding metaphor for the entire protocol.

**Your UTXO set is your pocket.** It contains exactly what you possess in this moment. Nothing more, nothing less. It is the only permanent data structure in the network.

**137 days of pruning is natural forgetting.** Just as you do not keep receipts from five months ago in your pocket, the network mathematically erases all transaction history after exactly 137 days (α⁻¹). Not because someone ordered it, but because the protocol makes it impossible to keep. No subpoenas, no archives, no eternal digital footprint.

**Stealth addresses ensure no one can look into your pocket.** Observers may see that coins are moving in the world, but they cannot see into your pocket — nor connect one payment to another. Your financial life remains private by default.

**Wallet recovery without history is finding your pocket again.** If you lose your phone or reinstall the software, you only need your 24-word seed. The protocol lets you scan the current UTXO set, recognize what belongs to you, and reclaim it — without needing any past transaction records. Your pocket is always recoverable from the present state alone.

**Rewards for simply being online is wearing the pants.** You do not need to actively trade, stake, or perform complex tasks. By keeping your node running — by simply being present — you earn a fair share. Like carrying your pocket with you through the day.

This is not a marketing slogan. It is the architectural soul of Elektron Net.

The network lives in the Here and Now, exactly like a Stoic who focuses only on what is within his power in the present moment. The mathematics secures your money. Time gently erases your traces. You remain free to act with what you truly own today.

> *"Mathematics secures your money.
> Time erases your traces.
> You own the moment."*

This pocket philosophy is not an afterthought. It is the reason the system was built the way it is — light, private, fair, and profoundly human.

## 1.2 – The Problem of the Digital Present

The internet was not built for people. It was built for institutions.

| | |
|---|---|
| Your bank account | can be frozen at any time, without reason |
| Your payment | requires a bank's permission |
| Your transaction | stored, analysed, reported |
| Your identity | owned by a corporation |
| You | are the product |

## 1.3 – The Vision

Elektron Net is a decentralised digital currency built on three principles:

**Light** – The network forgets everything after 137 days. Light in storage. Light in memory. Light in surveillance.

**Anonymous** – Every payment uses stealth addresses. No one can link your transactions. Not your government. Not your bank. Not anyone.

**Fair** – Every participant who stays online earns a fair share. The producer earns extra for doing the work. Everyone else earns together.

| | |
|---|---|
| Your wallet | yours forever, mathematically guaranteed |
| Your payment | direct, fast, anonymous |
| Your balance | cannot be frozen by anyone |
| Your rewards | earned by simply being online |
| The network | alive from block #1, with or without peers |
| Your history | forgotten after 137 days |
| You | are the owner |

## 1.4 – The Promise

> "A network where your wallet belongs to you.
> Where you are anonymous when you choose to be.
> Where nobody can freeze your money.
> Where nobody can trace your payments.
> Where you are not the product.
> Where mathematics governs, not institutions.
> Where a single node is enough to begin.
> Where 137 days later, everything is forgotten.
>
> Like the Lydian Elektron 2,600 years ago –
> value through mathematics, not through decree."

## 1.5 – The Two Numbers

**21,000,000** — Maximum Elek. Immutable forever. The same cap as Elektron. A commitment to scarcity and honesty.

**α⁻¹ = 137** — The fine-structure constant – the most fundamental dimensionless constant in physics. Describes the interaction of the electron with the electromagnetic field. *"A mystery ever since it was discovered."* – Richard Feynman

In Elektron Net, 137 is present throughout:
- 137 days → pruning window (forgotten by mathematics)
- 137 → maximum VRF committee size

---

# Part II – How It Works

> **Technical basis:** Elektron Net is implemented in **C++20** as a direct fork of **Bitcoin Core**. The full Bitcoin Core C++ codebase serves as the starting point; Elektron Net's protocol extensions are built on top.

# 3 – The Currency

## Units

| Unit | Symbol | Relation | Named After |
|------|--------|----------|-------------|
| Elektron | Elek | base unit | Lydian Elektron coin |
| Lepton | Lep | 1/100,000,000 Elek | Smallest Greek coin |
| Lepton Cent | cLep | 1/100 Lepton | Sub-unit precision |

`1 Elek = 100,000,000 Lep = 10,000,000,000 cLep`

## Maximum Supply

21,000,000 Elek. Anchored in Block #0. Enforced by every node on the network. Unchangeable by any person, company, or government.

This is not a promise. It is a mathematical property.

## The Emission Schedule

Every 60 seconds, a new block is produced. Every block creates new Elek. The amount decreases by half every 4 years – identical to Bitcoin's rhythm, because it works.

| Parameter | Value |
|---|---|
| Block time | 60 seconds |
| Blocks per day | 1,440 |
| Genesis block reward | 5.00000 Elek |
| Halving interval | 4 years (2,102,400 blocks) |

| Period | Block Reward | Per Day | Cumulative Total |
|---|---|---|---|
| Year 1–4 | 5.00000 Elek | 7,200/day | 10,512,000 |
| Year 5–8 | 2.50000 Elek | 3,600/day | 15,768,000 |
| Year 9–12 | 1.25000 Elek | 1,800/day | 18,396,000 |
| Year 13–16 | 0.62500 Elek | 900/day | 19,710,000 |
| Year 17–20 | 0.31250 Elek | 225/day | 20,367,000 |
| Year 21–24 | 0.15625 Elek | 113/day | 20,695,500 |
| … | … | … | … |
| ~Year 115 | ≈0.00000 | 0 | 21,000,000 max |

**No airdrop. No pre-mine. No founder allocation. No venture capital.**

Every Elek in existence must be earned by running the software. This is not charity. This is the only honest way.

---

# 4 – Technical Foundation

## Mathematics as Constitution

Security rests not on trust in institutions but on the mathematical unsolvability of certain problems. This is not faith – it is 50 years of cryptographic research confirmed by numerous attacks.

## Curve25519

```
y² = x³ + 486662x² + x  (mod 2²⁵⁵ - 19)

128-bit security. Fastest secure curve for ECDH.
Designed by D.J. Bernstein, 2006.

private_key = random 256-bit number
public_key  = private_key × G

pubKey from privKey: milliseconds
privKey from pubKey: longer than the age of the universe
```

## Ed25519 – Signatures

```
Sign:    sig = Ed25519_Sign(private_key, BLAKE3(message))
Verify:  Ed25519_Verify(public_key, BLAKE3(message), sig)

128-bit security. Deterministic. ~100,000 signatures/second.
```

## BLAKE3 – Hashing

Fastest modern cryptographic hash function. Used for wallet addresses, block hashes, Merkle roots, PoW, stealth addresses, and checkpoint digests.

## Merkle Trees

```
Block transactions:
  H(A)  H(B)  H(C)  H(D)
     \  /       \  /
    H(AB)       H(CD)
        \       /
        H(ABCD) ← Merkle Root

To prove Tx_A exists: provide H(B) and H(CD).
No other transaction revealed. Tamper-proof.
```

## Technology Stack

**BASE:** Foundation: Bitcoin Core (C++ fork) — all P2P, storage, and chain infrastructure is inherited directly from Bitcoin Core.

**From Bitcoin Core (already in codebase):**

| Component | Details |
|---|---|
| Language | C++20 |
| P2P Network | Bitcoin Core net.cpp (TCP/IP, peer discovery, message relay) |
| Storage | RocksDB / LevelDB (embedded key-value database) |
| Serialization | Bitcoin Core serialisation framework |
| BIP-32/39 | HD wallet derivation (already implemented) |
| Bech32m | Address encoding ("elek1" prefix — adapted) |
| JSON-RPC | Port 9337 (adapted from Bitcoin Core RPC infrastructure) |
| Qt GUI | Qt-based desktop wallet (adapted from Bitcoin Core Qt codebase) |
| Pruning | Bitcoin Core pruning framework + mandatory age-based deletion: blocks older than 197,280 slots (137 days) are always removed, independent of disk usage (validation.cpp FlushStateToDisk) |

**New C++ code (added on top of Bitcoin Core):**

| Component | Details |
|---|---|
| Cryptography | Ed25519 via libsodium (OP_CHECKSIG_ED25519 = 0xbb, OP_CHECKSIGVERIFY_ED25519 = 0xbc) |
| Hashing | BLAKE3 C library (addresses, stealth derivation, heartbeat HMAC, ChannelID, OP_HASH256_BLAKE3 = 0xbd) |
| VRF | IETF ECVRF via libsodium (crypto_vrf_prove/verify, ristretto255); Alpha = BLAKE3(prev_block_hash \|\| slot_le64); Proof = 80 bytes, Beta = 32 bytes |
| BLS | blst C-bindings (PBFTAggregateSig, DST "ELEKTRON_PBFT_BLS_SIG_v1"); #ifdef HAVE_BLST (Stub if blst not available) |
| pBFT | PBFTRoundState: IDLE→PREPARE→COMMIT→FINAL; Quorum: VRF_PBFT_QUORUM = 92 of VRF_COMMITTEE_SIZE = 137 |
| Stealth | Dual-key (scan/spend), Ed25519→x25519 conversion for ECDH |
| Bloom-Filter | StealthRecoveryFilter: 2^24 Bit (2 MB), 10×MurmurHash3 |
| Heartbeats | HeartbeatMsg (72 B), BLAKE3_keyed HMAC, epoch-rotating Secret; HeartbeatSender (60s Scheduler), HeartbeatTracker (Mutex-protected); FULL_NODE_SLOT_THRESHOLD = 197,280 Slots (137 days × 1440) |
| Channels | ChannelID = BLAKE3(txid\|\|vout), MAX_HTLC_PER_CHANNEL = 137; Script-Templates: BuildFundingScript, BuildHTLCOfferedScript etc. |
| HTLC-Routing | FindRoute() — Backward-Dijkstra, HOP_PENALTY = 0.01; HTLCForward::Settle() — BLAKE3-Preimage-Verification |
| Bootstrap | FetchBootstrapPeers() + RegisterWithBootstrap(), HTTP/1.1 Port 80; threadBootstrap in CConnman, -nobootstrap disables |
| Rewards | 25% producer / 75% epoch pool in miner.cpp, ELEKTRON_EPOCH_LENGTH |

---

# 5 – Wallet System

## Philosophy: Identity-Free by Design

Private key → Public key → Address → Balance. No username. No account. No registration required.

| Property | Value |
|---|---|
| Address encoding | Bech32m with prefix "elek1" |
| HD derivation | BIP-44/84/86 standard paths (same as Bitcoin Core) |
| Stealth spend key | m/137'/0'/0' (Elektron Net extension) |
| Stealth scan key | m/137'/0'/1' (Elektron Net extension) |

## Wallet Creation

Elektron Net uses Bitcoin Core's proven descriptor-wallet architecture
unchanged. The only addition is the automatic derivation of stealth keys
from the same HD master seed.

**Wallet Creation Flow** (identical to Bitcoin Core + stealth extension):

1. **Generate master seed** — 256-bit entropy via CSPRNG. The Bitcoin Core Qt client displays a 24-word BIP-39 mnemonic for the user to write down — the only backup needed.

2. **Derive descriptor set** (Bitcoin Core standard) — From master seed, BIP-32 derives one `DescriptorScriptPubKeyMan` per output type. Default address type: bech32m (P2TR, same as BC).

   | Path | Type | Note |
   |------|------|------|
   | `m/44'/0'/0'` | P2PKH | legacy, backward-compat only |
   | `m/49'/0'/0'` | P2SH-P2WPKH | |
   | `m/84'/0'/0'` | P2WPKH | bech32 |
   | `m/86'/0'/0'` | P2TR | bech32m — default |

3. **Derive stealth keypair** (Elektron Net extension) — `spend_key = derive(master, m/137'/0'/0')`, `scan_key = derive(master, m/137'/0'/1')`. Stealth address = `Bech32m("elek1st", scan_pub || spend_pub)`.

4. **Wallet active** — address usable immediately. No on-chain registration required for normal use.

**What changes vs. Bitcoin Core:** only Step 3. Everything else is
unchanged Bitcoin Core behaviour.

## Participant Registration and Epoch Rewards

Both regular users and Full Nodes register to participate in the VRF epoch
pool. Registration is **lightweight** and **not required** to use the wallet.
**Registration** (every Full Node participant):

**Full Node** (`NODEREGISTER` / `elkreg`, P2P): `{ pubkey[32] || payout_stealth_addr[64] || sig_ed25519[64] }`
- Requirement: 137 days of heartbeats → VRF committee eligibility
- Desktop / laptop / server: every node runs the full protocol
- Eligible to become block producer → earns the 25% share when selected
- Access to the 75% epoch pool (same as all other nodes)

**Anti-Sybil:**
- No balance requirement — anyone can register and earn from day one
- Must be online each epoch (send a heartbeat) to be in the VRF draw
- Sybil resistance comes from online presence: running 1000 simultaneous wallet instances requires 1000 active connections and heartbeats — practical cost without any artificial token gate
- One registration per stealth address (re-registration is idempotent)

**Reward Distribution** (Automatic — no claim required):
- At each epoch boundary (every 10 blocks = 10 minutes):
  - Pool = 0.75 × sum of block rewards in the epoch
  - VRF selects up to 21,000 recipients from ALL registered participants (Full Nodes and regular users are in the same pool — no distinction)
  - Each selected participant receives: pool / min(registered, 21000)
- Rewards go directly to the registered stealth address
- Producer creates ONE epoch-reward transaction (max 21,000 outputs)
- No claiming, no window, no roll-over — automatic at epoch end

This means: **every Elek holder participates on equal terms.**
Running a Full Node gives only one additional benefit: the chance to become
a block producer and earn the 25% producer share. For the 75% pool, Full
Nodes and regular users are treated identically.

## Stealth-Integrated Wallet

Every wallet automatically derives a dual-key stealth structure from the
same BIP-32 master seed. No extra seed or separate backup is needed.

```
DERIVATION (from BIP-32 master seed, same as all other keys):

  spend_privkey, spend_pubkey = HD-derive(master, m/137'/0'/0')
  scan_privkey,  scan_pubkey  = HD-derive(master, m/137'/0'/1')

  Published stealth address:
    Bech32m("elek1st", scan_pubkey[32] || spend_pubkey[32])
    → a 103-character string the user shares publicly

SENDING TO A STEALTH ADDRESS:
  Sender calls DeriveStealthPaymentScript(addr, &ephem_R):
    1. Generate ephemeral keypair (r, R = r·G_curve25519)
    2. ECDH: dh = r · scan_pubkey_x25519
    3. Tweak: t = BLAKE3(dh || R)
    4. One-time key: P = spend_pubkey + t·G_ed25519
    5. Output script: <P[32]> OP_CHECKSIG_ED25519
    6. OP_RETURN with R so recipient can detect the payment

RECEIVING (wallet scan):
  Wallet calls ScanStealthOutput(scan_key, spend_pub, script, R):
    1. ECDH: dh = scan_privkey · R_x25519  (symmetric to sender)
    2. Tweak: t = BLAKE3(dh || R)
    3. Expected: P = spend_pubkey + t·G_ed25519
    4. If script == <P> OP_CHECKSIG_ED25519 → match
    5. Spending key: k = spend_privkey + t (mod curve_order)

PRIVACY PROPERTY:
  Every payment uses a fresh one-time key P.
  An observer sees only <P> OP_CHECKSIG_ED25519 — P cannot be
  linked to the recipient's stealth address or to any other payment.
```

All on-chain receipts use stealth by default. Standard bech32m (P2TR)
addresses remain available for backward compatibility with wallets and
exchanges that do not yet support the Elektron Net stealth protocol.

---

# 6 – Network Architecture & Peer-to-Peer

## Bitcoin Core as Foundation

Elektron Net is a direct fork of Bitcoin Core (C++) and builds on its proven peer-to-peer network layer. All core components are implemented in C++ and derived directly from the Bitcoin Core source code:

**From Bitcoin Core** (C++ fork — preserved as-is):
- P2P message serialisation (magic bytes + command + payload)
- Block propagation (inv → getdata → block)
- Transaction relay (mempool sync)
- Headers-first sync
- DoS protection (banning, score-based)
- RocksDB / LevelDB storage layer
- Wallet BIP-32/39 derivation infrastructure

**Elektron Net Extensions** (new C++ code on top of Bitcoin Core):
- IPv6 as primary address family
- 60-second block time
- pBFT-style finality layer (VRF + BLS)
- VRF committee for block production
- Genesis Mode: single-node operation
- Stealth address relay (scan/spend keypair)
- 137-day pruning enforced at protocol level
- Heartbeat system (BLAKE3-HMAC, quantum-safe)
- Epoch reward distribution (25% producer / 75% pool)
- ElektronChannels (payment channels with built-in Watchtower)

## Node Architecture

Every participant in Elektron Net runs a Full Node by default — desktop,
laptop, server. The only exception are mobile light clients, which connect
through a trusted peer for reduced storage and bandwidth.

```
┌─────────────────────────────────────────────────────────────┐
│                    NODE ARCHITECTURE                         │
├─────────────────────────────────────────────────────────────┤
│  FULL NODE (Desktop / Laptop / Server — default)            │
│    - Runs the complete Elektron Net protocol                 │
│    - 137+ days continuous online (verified via heartbeats)  │
│    - Genesis Phase: ALL nodes are Full Nodes               │
│    - Forms VRF committee (137 randomly selected per slot)   │
│    - Signs blocks and checkpoints (BLS aggregate)           │
│    - Can become block producer → earns the 25% share        │
│    - Access to the 75% epoch pool                          │
│    - Enforces the 137-day pruning window at protocol level │
├─────────────────────────────────────────────────────────────┤
│  MOBILE CLIENT (Light — exception)                         │
│    - Connects through a trusted Full Node peer               │
│    - Does NOT store the full UTXO set                        │
│    - Cannot produce blocks or sign checkpoints              │
│    - Cannot join the VRF committee                          │
│    - Receives epoch pool rewards via trusted peer relay     │
└─────────────────────────────────────────────────────────────┘
```

**Full Node Qualification:**
- Minimum: 137 days of online time recorded (heartbeats)
- Heartbeats continue while online to maintain qualification
- Genesis Phase: all nodes are automatically Full Nodes
- No voluntary "enable" step — every desktop client is a Full Node by default

**Full Node Status (Three States):**

| State | Condition | Counts for Quorum | Block Producer (25%) | Epoch Pool (75%) | Reactivation |
|-------|-----------|-------------------|----------------------|------------------|--------------|
| ACTIVE | FN + heartbeat this epoch | YES | YES (VRF committee) | YES (auto-paid) | — |
| DORMANT | FN registered, no heartbeat this epoch | NO | NO | NO | 1 heartbeat → ACTIVE |
| OFFLINE | FN registered, missed > 10 consecutive heartbeats | NO | NO | NO | re-register from genesis |

- **Online = eligible**: heartbeat within the current epoch (10 blocks) is required
  to appear in the VRF draw
- Offline = no reward — prevents Sybil attacks via mass offline registrations
- Reactivation to ACTIVE: instant (1 heartbeat) — no new 137-day wait
- Short outages (vacation, reboot): 1 heartbeat on return restores all eligibility

**Why This Design:**
- Short offline periods for FNs: instant recovery, NO penalty
- Network stability: Dormant FNs don't bloat the quorum
- Every node is a Full Node — no tier discrimination in the 75% pool
- No complex claim logic: epoch ends → automatic payout → done

## Bootstrap & Node Discovery

1. DNS seeds (config file — minimum 5, geographically distributed, user-editable)
2. Bootstrap Server (PHP-based registry at www.elektron-net.org/bootstrap.php)
3. addr relay (peers share known addresses)
4. peer.db (saved peer list from previous sessions)
5. Hardcoded fallbacks (localhost for testing)
6. Genesis Mode (if no peers found — single node fully operational)

### Bootstrap Server (PHP Registry)

Full Nodes register dynamically. Implemented in `src/node/bootstrap_client.h/cpp`:

| Endpoint | Method | Status | Description |
|----------|---------|--------|--------------|
| `/bootstrap.php?action=list` | GET | ✅ implemented | Fetch peer list |
| `/bootstrap.php?action=register` | POST | ✅ implemented | Register as node |
| `/bootstrap.php?action=heartbeat` | POST | ✅ implemented (Phase 2) | Refresh registry TTL every 30 min |
| `/bootstrap.php?action=stats` | GET | ✅ implemented (Phase 2) | Registry statistics |

- **Protocol:** Plain HTTP/1.1 Port 80 by default; HTTPS (TLS 1.2+) Port 443 when compiled with `-DWITH_BOOTSTRAP_TLS=ON` (links OpenSSL, verifies CA chain)
- **Host:** `www.elektron-net.org`, Path: `/bootstrap.php`
- **IP Support:** IPv4 and IPv6
- **Chain Filter:** Every request contains `genesis_hash` — only peers of the same chain are returned
- **Max Peers:** 64 per request (`BOOTSTRAP_MAX_PEERS`)
- **Timeout:** 10 seconds (`BOOTSTRAP_TIMEOUT_SEC`)
- **threadBootstrap:** Runs persistently (loops with 30 min sleep); initial fetch+register on start, then periodic `HeartbeatToBootstrap()` to keep TTL alive. Disabled via `-nobootstrap`
- **Registry stats** exposed via `getnetworkstats` RPC under `"registry"` key

## Genesis Node Chain Authentication (Genesis Hash Guard)

The first node ever to start on a given chain is the **Genesis Node**. Because
starting as Genesis Node creates a new, independent chain, the protocol enforces
an explicit authentication step to prevent both accidents and malicious chain forks.

### The Problem

Without a guard, any operator who finds no peers — due to a network outage,
misconfiguration, or deliberate isolation — could inadvertently fork the chain
by silently starting as Genesis Node. Other nodes would reject that fork, but
real damage (confusion, wasted blocks, split communities) could still result.

### The Mechanism

The Genesis Hash Guard is a **file-based check** executed every time the daemon
starts, implemented in `src/init.cpp`. There is no interactive menu.

**First start** (datadir is new):

The daemon computes the genesis block hash from the compiled-in chain parameters
and writes it to `<datadir>/genesis_hash.dat`. This file becomes the chain's
permanent fingerprint for this data directory.

**Every subsequent start**:

The daemon reads `genesis_hash.dat` and compares it byte-for-byte against the
expected hash. A mismatch — e.g. starting mainnet software against a testnet
datadir — triggers an `InitError` and the daemon **refuses to start**:

```
Genesis hash mismatch!  Stored: <old_hash>  Expected: <new_hash>
This datadir was initialised on a different chain.
Use a separate -datadir for each network, or delete the datadir to start fresh.
```

There is no automatic recovery. The operator must either use the correct datadir
or delete `genesis_hash.dat` to permit a fresh start.

### Chain Identification in Peer Communication

The genesis hash is not only checked locally. It is included in every outgoing
bootstrap request:

- **Bootstrap server registration:** `POST` body contains `genesis_hash`
- **Bootstrap peer list query:** `GET` query contains `genesis_hash=…`
- **P2P handshake** (Phase 1): nodes exchange genesis hashes; mismatches disconnect

This means the bootstrap server only returns peers that registered with the same
genesis hash. Nodes on a different chain are never returned, never connected to,
and never waste bandwidth.

### Network-Mode Behaviour

| Network | Behaviour on chain mismatch |
|---------|------------------------------|
| any | `InitError` — daemon refuses to start |
| first start | `genesis_hash.dat` created, hash recorded |
| reset | Delete `genesis_hash.dat` → next start is treated as first |

### Relation to Sybil Resistance

The two mechanisms protect against different threats:

| Mechanism | Protects Against |
|-----------|------------------|
| IPv6 subnet diversity enforcement | **Sybil:** mass identity from same IP range |
| Genesis Hash Guard (file check) | **Fork:** accidental or malicious datadir reuse |

The subnet diversity rule limits the influence of any single network location.
The Genesis Hash Guard ensures a datadir can only ever be used for one chain.
Neither mechanism can be circumvented without modifying and recompiling the binary.

### Why This Design

The genesis hash is publicly visible in the source code. Its protective value
does not come from secrecy — it comes from the **permanent file binding** between
a datadir and its chain. No automation, no misconfiguration, and no carelessness
can silently mix datadirs from different chains.

> **The genesis hash of Elektron Net mainnet:**
> `0e0033ee45b67f54c617399f01c23b3afe14e6b0de6d63ef3daf91bb15edb6d6`

---

## Genesis Mode: Solo Operation

If no peers are found and the Genesis Hash Guard is passed, a single node
operates alone:
- Produces blocks
- Validates transactions
- Earns all rewards
- Full functionality, no minimum required

This is not a fallback. This is a feature. The network is alive from block #1.

---

# 7 – Consensus: VRF Committee & Block Production

## Two Modes

The network operates in two modes depending on the number of Full Nodes.

### Genesis Mode (FN < 137)

When fewer than 137 Full Nodes exist:
- ALL nodes are Full Nodes (Genesis Phase - automatic)
- Simple rotation selects the block producer
- No VRF committee
- No BLS checkpoint signatures
- Ed25519 only
- Full reward to producer (25% + 75% pool)
- Automatic promotion of candidates if needed

### Phase 1 (FN ≥ 137)

When 137 or more Full Nodes are active:
- VRF committee selection (from Full Node Registry only)
- Fixed committee size: C = 137
- pBFT finality (≥2/3 signatures required = 92 votes)
- BLS aggregate signatures
- Only Full Nodes can participate in VRF committee

## Committee & Byzantine Threshold

| Parameter | Value |
|---|---|
| Committee Size | C = 137 (always maximum, when Phase 1 active) |
| Byzantine Threshold | T = ceil(C × 2/3) = 92 votes required |
| Tolerated Byzantine | 45 nodes (137 − 92) |

**Why 137 + 2/3?**
- VRF randomization makes coordinated attacks exponentially harder
- Even with 30% of network controlled by attacker
- Probability of 92 attackers in random 137-committee: negligible
- No fork needed: security is built-in from day one

## VRF Selection

Every slot, the committee is selected by VRF:

```
STAGE 1 — SELECTION:

  1. Each FULL NODE computes:
     vrf_input = hash(slot_number || last_checkpoint_hash || utxo_root)
     vrf_output = VRF_prove(node_privkey, vrf_input)

  2. Sort by vrf_output (lowest = best)

  3. Top C nodes = committee for this slot

  4. Within committee: P1 (committee[0]), P2 (committee[1]), P3 (committee[2])

NOTE: last_checkpoint_hash + utxo_root are used to prevent long-range
manipulation. The checkpoint hash changes every 137 days; the UTXO root
changes with every transaction, making manipulation impossible.

STAGE 2 — VOTING:

  5. Each committee member validates the block
     Every member has exactly 1 vote

  6. Threshold: T = ceil(C x 2/3) = 92 votes required for finality

  7. P2 collects signatures → BLS aggregate → final block
```

## Block Production Flow

```
Every 60 seconds:

  slot = unix_time / 60
  slot_start = slot × 60
  valid_production_window = [slot_start, slot_start + 45 seconds]

  t =  0s:   P1 produces and broadcasts
  t = 15s:   If P1 failed → P2 prepares
  t = 30s:   If P2 ready → P2 broadcasts
  t = 40s:   If P1,P2 failed → P3 broadcasts
  t = 60s:   If T signatures not collected → slot skipped

  Committee votes on the first valid block received.
  If ≥2/3 vote: block is final (pBFT safety guaranteed).
```

## Pacemaker

If slots are skipped:
```
k = consecutive skipped slots
timeout = 60s × 2^min(k, 10)
VRF committee is redrawn
Offline nodes are unlikely to be re-selected (VRF is independent per slot)
```

---

# 8 – The Reward System

*This is the heart of the protocol.*

## The Philosophy of Fairness

Elektron Net makes no future promises. Rewards are distributed fairly and instantly. A network lives in the *Now*. Anyone who is online today earns today. There are no complex calculations, no retroactive claims.

## The Rule

Every block reward is distributed as follows:

| Share | Recipient | Timing |
|---|---|---|
| 25% | Block Producer | paid instantly, every block |
| 75% | Pool | paid every 10 minutes, at epoch end |

## Why This Model

| Principle | Explanation |
|---|---|
| Simplicity | Only two numbers (25% and 75%) |
| Fairness | Everyone online shares the pool |
| Incentive | Producer gets extra for doing the work |
| Scalability | 21,000 node cap prevents chain bloat |
| Efficiency | 10-minute batching = 10x less UTXOs |

## Online Definition (Rolling Window)

```
A node is "online" if it participated in consensus during the current slot
OR in one of the last K slots (S-K to S-1).

K = 10 slots (10 minutes at 60s block time)

Participation = one of:
  1. Selected in the VRF committee, OR
  2. Produced or validated a block, OR
  3. Sent a heartbeat message (included in block)
```

## Heartbeat Mechanism (Quantum-Safe)

The heartbeat system uses BLAKE3-based HMAC — no ECC key exchange, minimal quantum computer exposure.
Implemented in `src/node/heartbeat.h/cpp`, `heartbeat_sender.h/cpp`, `heartbeat_tracker.h/cpp`.

```
DESIGN PRINCIPLES:
  - Quantum-safe: BLAKE3-HMAC, no ECC key exchange in heartbeat
  - Minimal storage: only pubkey[32] + hmac_hash[32] + slot[8]
  - Minimal computation: 1× BLAKE3_keyed per heartbeat
  - No IP storage: GDPR/DSGVO Art. 5(1)(c) data minimisation

MESSAGE FORMAT (HeartbeatMsg, 72 bytes):
  slot[8]         uint64_le   Current block height (= slot)
  hmac_hash[32]   [u8;32]     BLAKE3-HMAC commitment
  node_pubkey[32] [u8;32]     Ed25519 identity pubkey

ALGORITHM:
  IDENTITY KEY (once per node):
    Stored in: <datadir>/heartbeat_identity.dat
    Created via:   randombytes_buf() (libsodium CSPRNG)
    Pubkey derived via: crypto_sign_ed25519_seed_keypair()

  PER EPOCH (epoch = slot / 1440):
    secret = BLAKE3( identity_privkey[32] || epoch_le64[8] )
    // Rotates daily — limited retrospective impact if compromised

  PER SLOT (every 60 seconds, HeartbeatSender::Send()):
    hmac_hash = BLAKE3_keyed( secret[32], slot_le64[8] )
    msg = { slot, hmac_hash, node_pubkey }
    P2P broadcast to all connected peers

RECEPTION (HeartbeatTracker::ProcessHeartbeat()):
  Structure check: hmac_hash ≠ 0, node_pubkey ≠ 0
  NodeRecord updated: consecutive_slots, last_seen_slot
  is_full_node = true  if  consecutive_slots ≥ 197,280
  MAX_MISSED_SLOTS = 10  (allowed gap per streak)

WHY QUANTUM-SAFE:
  - BLAKE3 is a hash function (no ECC)
  - The HMAC secret never leaves the node (only the hash is transmitted)
  - Even if Ed25519 is broken by a quantum computer:
    heartbeat liveness proof remains secure (pure BLAKE3-HMAC chain)
  - Secret rotates per epoch → limited damage if compromised
```

## Node Cap & Epoch Batching

**Reward Cycle:** Every 10 minutes (1 epoch = 10 blocks)
- Up to 21,000 DIFFERENT nodes receive rewards EACH epoch
- Next epoch: potentially different 21,000 nodes
- Every epoch is independent — your odds reset each time

To prevent chain bloat:

- **Node Cap:** Maximum 21,000 nodes receive pool rewards per epoch. If `online_nodes > 21,000`: selection by deterministic VRF.
- **Epoch:** 10 blocks (10 minutes). At epoch end: ALL pool rewards aggregated into ONE transaction. Result: max 21,000 UTXOs per epoch (instead of 21,000 per block).

**Selection Transparency:** VRF input = `hash(slot_number || last_checkpoint_hash)`. All nodes can independently verify the selection result. The producer cannot manipulate which nodes are selected — VRF output depends only on publicly known inputs.

**Comparison — Chain Bloat Prevention:**

| Scenario | UTXOs/day |
|---|---|
| Without batching | 21,000 × 1,440 blocks = 30,240,000 |
| With 10-min epoch | 21,000 × 144 epochs = 3,024,000 |
| Reduction | 10x smaller ✓ |

## Reward Distribution Timeline

```
BLOCK PRODUCTION (every 60 seconds):
  Producer receives: 25% of block_reward IMMEDIATELY
  Pool share (75%): accumulates for 10 blocks

EPOCH END (every 10 blocks = 10 minutes):
  1. Sum all accumulated pool shares (10 blocks × 75%)
  2. Count unique online nodes (max 21,000)
  3. per_node = pool_sum / min(online_count, 21000)
  4. Create 1 UTXO per eligible node
```

## Examples

```
Scenario 1: Solo node (N = 1)
  Block reward: 5 Elek
  Producer receives: 25% + 75% = 100% = 5 Elek
  Pool: 0 (only producer)

Scenario 2: 10 nodes online, epoch reward = 37.5 Elek
  Pool per node: 37.5 / 10 = 3.75 Elek
  Producer: 1.25 (instant) + 3.75 (epoch) = 5 Elek ✓

Scenario 3: 100 nodes online
  Pool per node: 37.5 / 100 = 0.375 Elek
  Producer: 1.25 + 0.375 = 1.625 Elek

Scenario 4: 1,000,000 nodes online (capped at 21,000)
  Pool per node: 37.5 / 21000 = 0.00178 Elek
  Producer: 1.25 + 0.00178 = 1.25178 Elek
```

## Scalability – 10-Year Projection

**Chain size after 10 years:**

| Component | Size |
|---|---|
| Blocks | 5,256,000 (at 60s intervals) |
| Block data (rolling 137-day window) | ~40 GB |
| Block headers | ~420 MB |
| UTXO Set | ~5 GB |
| Checkpoints | ~1 MB |
| **Total permanent storage** | **~5.5 GB** |
| Bitcoin (same timeframe) | ~550 GB |

Elektron Net is ~100x smaller due to 137-day pruning.

**Why this is possible:**
- 137-day pruning deletes all transaction history automatically
- Only UTXO set, headers, and checkpoints are permanent
- 21,000 node cap limits reward UTXOs to 3M/day (not billions)

## Transaction Fees

Market-based. Senders attach fees to incentivise block inclusion. Block producer collects 100% of fees in their block. No burning. No protocol treasury.

### Dynamic Fee Policy

Elektron Net extends Bitcoin's standard fee mechanism with a two-parameter **Dynamic Fee Policy** that keeps transaction costs practical regardless of Elek's fiat valuation:

| Parameter | Default | Description |
|---|---|---|
| `min_relay_fee_floor` | **100 cLep/kvB** (= 1 Lep/kvB) | Minimum fee to relay a transaction. 100× lower than Bitcoin's 100 sat/kvB floor. |
| `max_fee_ratio_cap` | **10 000 ppm** (= 1%) | Fee is capped at 1% of the primary output value. Prevents fees from being disproportionate at high coin valuations. |

**Example at 1 Elek = 100 000 €:**
- 1 Lep = 0.001 € (1/100 000 000 of 100 000 €)
- Minimum fee for a 250-byte transaction: 25 cLep = 0.25 Lep = 0.00025 €  ≈ 0.025 cents
- Sending 5 € (5 000 000 Lep): max fee = 1% = 50 000 Lep = 0.05 € ✓

**Bitcoin's existing mechanism** (`-minrelaytxfee`, `-blockmintxfee`, fee estimation) is reused as the underlying infrastructure. The Elektron extensions add sub-Lep (cLep) granularity and the ratio cap on top.

**Configuration** (startup args):
```
-elektronfeefloor=<cLep/kvB>    Override the minimum relay fee floor
-elektronmaxfeeratio=<ppm>      Override the max fee ratio cap (0 = disable)
```

**Runtime adjustment** (RPC, operator use):
```
getfeepolicy                    Show current fee policy settings
setfeepolicy <floor> <ppm>      Update fee policy without restart
```

---

# 9 – Privacy & Pruning

## The Philosophy of "Pants & Pocket"

Elektron Net is modeled after real, physical life. When you put on pants in the morning and find coins in your pocket, only the present moment matters:

- **The Current State:** You know how many coins you have *right now*.
- **The Sovereignty:** Because it is your pants, you know the coins are yours.
- **The Forgetfulness:** You don't need to remember what you bought at the supermarket months ago.
- **The Focus:** What matters is only what you possess in the present moment to be capable of acting.

This human experience is translated directly into code.

## The Core Principle

Elektron Net does not store transaction history indefinitely. Transaction data is deleted after exactly 137 days – by mathematics, not by policy.

This is not GDPR compliance. It is a structural impossibility of data retention. No company can be subpoenaed. No server can be hacked. No government can demand records because those records simply no longer exist.

### UTXO as the "Pocket"

The network permanently stores the current status of all unspent amounts (UTXOs). This corresponds to reaching into your pocket:
- It shows your current balance in the "Here and Now."
- Mathematical security (Ed25519) replaces the physical feeling of the cloth pocket — only you have access.

### The 137-Day Pruning (The Forgetting)

Like in real life, the network deletes history after 137 days (α⁻¹).
- **No Archive:** There is no permanent record of your past spending for institutions.
- **Stoic Calm:** The system frees the user from the burden of an eternal digital file.

*"Mathematics secures your money. Time erases your traces. You own the moment."*

## Stealth Addresses

Every on-chain payment uses stealth addresses by default. Observers see only one-time addresses that cannot be linked to any recipient wallet or to each other.

```
SETUP (once per wallet):
  Recipient publishes scan_pubkey and spend_pubkey

PER PAYMENT:
  Sender generates ephemeral keypair (r, R = r×G)
  one_time_address = BLAKE3(r × scan_pubkey) × G + spend_pubkey
  Transaction sent to one_time_address; R included in tx data

RECIPIENT DETECTION (Bloom-Filter, StealthRecoveryFilter):
  Instead of scanning every transaction, the wallet uses a Bloom-Filter
  for efficient, privacy-preserving detection.

  FILTER PARAMETERS (src/wallet/stealth_spkm.cpp):
    Size:    FILTER_BITS = 2^24 = 16,777,216 bits = 2 MB (fixed)
    Hashes:  10 × MurmurHash3 with domain-separated seeds
    Seed_i = i × 0xFBA4C795 + 0x656C656B  ("elek")
    FPR:    ~0.045% at 1M candidates

  STEP 1 – Build filter (StealthRecoveryFilter::Build):
    scan_x25519 = Ed25519→Curve25519(scan_privkey)
    For N = 0 to num_candidates (~1M default):
      r_N    = BLAKE3(scan_privkey || N_le64) [x25519-clamped]
      R_N    = r_N × G (Curve25519)
      dh     = scan_x25519 × R_N  (ECDH shared secret)
      tweak  = BLAKE3(dh || R_N)
      P      = spend_pubkey + tweak×G  (Ed25519 addition)
      filter.Insert(P)  → 10 MurmurHash3 bits set

  STEP 2 – Scan outputs (ScanForStealthOutputs):
    Per transaction: collect OP_RETURN outputs with R
    Per output in format <key[32]> OP_CHECKSIG_ED25519:
      IF !filter.MightBeOurs(key) → skip
      Otherwise: full ECDH verification (ClaimOutput)

  STEP 3 – Full verification (ClaimOutput):
    Computes tweak via x25519-ECDH + BLAKE3
    On match: spend_scalar stored → output spendable

  PERFORMANCE:
    UTXO Set = 10M outputs
    Bloom-Filter = 2 MB (fixed, independent of network size)
    Lookups = 10M × O(1) ≈ 100 ms
    False positives at 1M candidates: ~450 (0.045%)

  SCALABILITY:
    The filter contains ONLY this wallet's possible addresses —
    does not grow with the network UTXO set.
    At 100M UTXOs: still 2 MB filter, ~4,500 full verifications.

  SECURITY NOTES:
    - Bloom-Filter generation is 100% local (no data leaves the device)
    - Filter reveals nothing about wallet contents to network observers
    - An observer seeing the filter cannot derive any addresses
    - Filter can be regenerated from seed at any time

OBSERVER:
  Sees:    R, one_time_address, hidden amount
  Cannot:  link address to recipient
  Cannot:  link two payments to same recipient
```

## What Each Node Stores

| Retention | Data |
|---|---|
| **Always (permanent)** | Genesis block header (80 bytes, never deleted) |
| | All block headers (80 bytes each, chain integrity) |
| | UTXO set (current balances with stealth metadata) |
| | Checkpoint chain (BLS-signed by committee, one per pruning period) |
| **137 days (then deleted)** | Full transaction content |
| | Input/output details |
| **Never** | User identity |
| | IP-to-wallet mapping |
| | Permanent transaction history |

## Checkpoint Trust Model

```
WHY CHECKPOINTS?

  After 137 days, all transaction history is pruned.
  New nodes need a way to verify the UTXO set is correct.

HOW IT WORKS:

  Every 137 days (197,280 blocks):
    1. Current VRF Committee (137 members) creates a Checkpoint
    2. Checkpoint contains:
       - Block height
       - UTXO Root Hash (Merkle root of all current UTXOs)
       - BLS Signatures from committee members
    3. Threshold: 92 of 137 signatures required (2/3 Byzantine))

TRUST MODEL:

  - Genesis Block: First block created by the first node, never deleted
    Its header contains the initial UTXO Root Hash

  - Checkpoints: Signed by VRF Committee (92/137 required)
    - Committee is randomly selected from ≥137 nodes
    - 92 honest signatures = trustworthy
    - Attacker needs 28+ colluding nodes to forge

RECOVERY PROCESS FOR NEW NODES:

  1. Download Genesis Block header from any peer
     → Contains initial UTXO Root Hash

  2. Download latest Checkpoint
     → Verify: 92/137 BLS signatures valid
     → Verify: UTXO Root matches expected value

  3. Download UTXO set from multiple peers (3+ sources)
     → Compare UTXO Root Hash across peers
     → If mismatch: block peers, try different ones
     → If match: UTXO set is verified

  4. Verify UTXO set integrity:
     → Recalculate Merkle root of downloaded UTXO set
     → Must match Checkpoint's UTXO Root Hash

WHY THIS IS SECURE:

  - Cannot forge Checkpoint without 92/137 committee members
  - Committee is randomly selected per slot (VRF)
  - UTXO Root comparison across peers detects tampering
  - No single point of trust required
```

## Storage Requirements

| Component | Year 1 | Year 5 | Year 10 |
|-----------|--------|--------|---------|
| UTXO Set | ~50 MB | ~1-2 GB | ~3-6 GB |
| Block Headers | <1 MB | ~12 MB | ~24 MB |
| Checkpoint | <1 MB | <1 MB | <1 MB |
| **Total** | **~100 MB** | **~2-3 GB** | **~5-8 GB** |

Light Mode: ~1 GB RAM, ~1 GB storage

## Wallet Recovery Without History

```
STEP 1: Enter 24-word seed phrase
        → Derive: scan_privkey (m/137'/0'/1'), spend_pubkey (m/137'/0'/0')

STEP 2: Generate Bloom-Filter locally (StealthRecoveryFilter::Build)
        → Size: 2 MB (FILTER_BITS = 2^24), 10 × MurmurHash3
        → scan_privkey → x25519, then per candidate:
          r_N = BLAKE3(scan_privkey || N_le64) → R_N → ECDH → tweak → P
        → No network connection needed

STEP 3: Download current UTXO set from peers
        → Everyone has it, no special trust required
        → Only UTXO data, no private information

STEP 4: Apply Bloom-Filter to UTXO set (ScanForStealthOutputs)
        → O(n) with minimal overhead (10M UTXOs in ~100ms)
        → FPR ~0.045% at 1M candidates: ~450 false positives

STEP 5: Verify each candidate (full stealth derivation)
        → Confirm true positives
        → Discard false positives

STEP 6: Derive spend keys for confirmed UTXOs
        → spend_key = BLAKE3(scan_privkey × R) + spend_privkey

STEP 7: Wallet recovered
        → All funds accessible
        → No transaction history needed
        → Works after 137 days offline
        → 100% self-verified, no trusted third party
```

**Why this is secure:**
- The Bloom-Filter reveals NO information about your addresses to the network
- UTXO set download is read-only (you send nothing)
- All verification is local
- No server or oracle required

This is the power of privacy by mathematics. The network forgets. But you can always remember.

## The "Here and Now" Principle

| Question | Answer |
|---|---|
| What is my balance? | Check the current UTXO set. |
| Can I spend? | Yes, if the UTXO exists. |
| How do I recover? | Enter seed → Generate Bloom-Filter locally → Scan UTXO set → Verify candidates → Done. |
| Is my history gone? | From the network — yes. From you — no. |

---

# 10 – Payments

## On-Chain Payments

Standard wallet-to-wallet payments:
- Sender creates transaction (stealth address by default)
- Producer includes in block
- 60-second finality
- Private by default

## QR Code Payments

Every wallet supports receiving and sending payments via QR codes, following the BIP-21 standard used by Bitcoin.

**QR Code Format** (`elek:` URI scheme — BIP-21 compatible):

`elek:<address>?amount=<elek>&message=<text>`

| Component | Description |
|---|---|
| `address` | Bech32m "elek1..." stealth address |
| `amount` | Optional, in Elek (not Lep) |
| `message` | Optional, payment description (max 128 chars) |

**Example:** `elek:elek1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9x7d9?amount=0.5&message=Coffee`

### Receiving via QR Code

1. **Generate QR Content** — User taps "Receive" in wallet. System generates: `address` = wallet's current stealth address, `amount` = (optional) user-entered amount, `message` = (optional) user-entered description, `uri = "elek:" + address + "?" + params`.
2. **Encode as QR** — Encode URI as UTF-8 bytes, generate QR code matrix (version 1–40), error correction: Level M (~15%), render as QR image.
3. **Display** — Show QR code on screen, show address as text below (copy button), show amount and message if entered. User can share or save QR image.

**QR Code Sizes:**

| Content | Version | Modules |
|---|---|---|
| Address only | ~Version 2 | 25×25 |
| With amount | ~Version 4 | 29×29 |
| With amount + message | ~Version 7 | 37×37 |

All use Level M error correction (15% recovery). Scanning distance: 0.5–1 m typical, 0.2–0.5 m with small QR.

### Sending via QR Code

1. **Scan QR** — User taps "Send" → "Scan QR". Camera activates, detects and decodes QR code, parses `elek:` URI.
2. **Parse URI** — Validate: address starts with `"elek1"`, address is valid Bech32m (checksum), amount (if present) is positive number, message (if present) is ≤ 128 chars. If invalid → show error, abort.
3. **Confirm Payment** — Display parsed payment: To (address, first/last 6 chars shown), Amount, Message. User confirms → proceed.
4. **Create Transaction** — Build transaction to stealth address, sign with Ed25519, broadcast to network.
5. **Status** — Show "Payment sent" with txid, monitor for confirmation.

### Offline QR Codes (Payment Requests)

For merchants or recurring payments, QR codes can be pre-generated offline.

**Static QR** (no amount): User generates QR with only address. Payer enters amount manually. Use case: donation addresses, tip jars.

**Dynamic QR** (with amount): User generates QR with address + specific amount. Amount is locked in QR. Use case: point-of-sale, invoices.

**Invoice QR**: Merchant's server generates unique address per invoice. QR contains: address + amount + description + expiry. Payer scans → confirms → pays. Invoice marked paid when tx confirms.

### BIP-21 Compatibility

Elektron Net QR codes follow the BIP-21 URI scheme, making them readable by any BIP-21 compatible wallet.

- **Standard BIP-21 (also works):** `elektron:1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa?amount=0.5&label=Alice`
- **Elektron Net equivalent:** `elek:elek1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq9x7d9?amount=0.5&label=Alice`

## ElektronChannels: Payment Channels

For fast, low-cost payments between two parties without touching the blockchain for every transaction.

```
CHANNEL LIFECYCLE:

  Open:    Both parties fund a 2-of-2 Ed25519 output on-chain
           ChannelID = BLAKE3(funding_txid[32] || vout_le32[4])
           Funding script: <pubA> OP_CHECKSIGVERIFY_ED25519
                           <pubB> OP_CHECKSIG_ED25519  (68 Bytes)

  Use:     Exchange signed ChannelState objects off-chain (instant)
           sequence_number: monotonically increasing — higher number wins on-chain
           Both parties sign each state (sig_local[64], sig_remote[64])
           is_confirmed = true once both signatures are present

  Close:   Cooperative: BuildCoopCloseOutputScript per party (34 bytes each)
           Unilateral:  BuildCommitmentOutputScript (OP_IF revocation /
                        OP_ELSE CSV-delayed)
           Dispute:     CHANNEL_DISPUTE_BLOCKS = 36 blocks

SECURITY PARAMETERS:
  MAX_HTLC_PER_CHANNEL   = 137  (α⁻¹, maximum parallel HTLCs)
  CHANNEL_DISPUTE_BLOCKS = 36   (Watchtower window = 36 minutes)
  MIN_CHANNEL_CAPACITY   = 1000 Satoshi
```

## HTLC: Multi-Hop Routing

HTLCs enable payments through intermediary channels without trusting them.

```
Alice → Bob → Carol → Dave (no direct channel required)

  1. Dave generates secret R, shares H = BLAKE3(R) with Alice (invoice)
  2. Alice routes via Dijkstra: FindRoute(table, Alice, Dave, amount, height)
     Cost metric: total_fees + 0.01 × hop_count (HOP_PENALTY)
     Algorithm: backward Dijkstra from recipient to sender
     Max hops: 20, BASE_CLTV_EXPIRY: 40 blocks

  3. Alice → Bob → Carol → Dave: HTLCs with decreasing CLTV expiry
  5. Dave reveals R → BLAKE3(R) == payment_hash confirmed
     Each hop: HTLCForward::Settle(r_preimage) verifies via BLAKE3

Result: atomic. Either all hops succeed, or all refund on timeout.

max_pending_htlc_per_channel = 137 (α⁻¹)

HTLC OUTPUT SCRIPT (implemented in BuildHTLCOfferedScript):
  OP_IF
    OP_SIZE 32 OP_EQUALVERIFY
    OP_HASH256_BLAKE3 <payment_hash[32]> OP_EQUALVERIFY
    <receiver_pubkey[32]> OP_CHECKSIG_ED25519
  OP_ELSE
    <timeout_height> OP_CHECKLOCKTIMEVERIFY OP_DROP
    <sender_pubkey[32]> OP_CHECKSIG_ED25519
  OP_ENDIF

New opcodes: OP_HASH256_BLAKE3 (0xbd), OP_CHECKSIG_ED25519 (0xbb)
```

## Lepton Streams: Continuous Micropayments

**Implementation:** `src/channels/lepton_stream.h/cpp` (Phase 2)

**Stream types:**
- `PER_SECOND` — per-second billing (video streaming, voice calls)
- `PER_KB` — per-kilobyte billing (data transfer, file sync)
- `PER_REQUEST` — per-call billing (API metering, compute)

**Sub-unit:** cLep (centi-Lep). `1 Lep = 100 cLep`. cLep accumulate off-chain
inside `StreamManager`; only whole Lep settle into the channel balance via
`MarkSettled()`. On-chain settlement uses the existing ElektronChannel state.

**Lifecycle:**
1. `elkstrmopen` P2P message — sender opens stream; `StreamID` derived as
   `BLAKE3(channel_id[32] ‖ payer_pubkey[32] ‖ start_time[8])`
2. `elkstrmtick` P2P message — periodic usage tick; `StreamManager::RecordUsage()`
   accumulates cLep; whenever `PendingSettlement() ≥ 100 cLep`, `MarkSettled()`
   increments the channel balance by whole Lep
3. `elkstrmclose` P2P message — either party closes; `FinaliseClose()` settles
   remaining balance and marks the stream `CLOSED`

**Data structures:**
- `StreamID` — 32-byte BLAKE3-derived identifier
- `StreamType` — `PER_SECOND | PER_KB | PER_REQUEST`
- `StreamStatus` — `OPEN | CLOSING | CLOSED`
- `LeptonStream` — rate (`rate_clep_per_unit`), accumulators
  (`metered_units`, `settled_lep`), payer/payee Ed25519 pubkeys
- `StreamManager` — thread-safe registry; `OpenStreamCount()`, `TotalSettled()`,
  `GetOpenStreams()`; exposed in `NodeContext::stream_manager`

**Developer API:** `liststreams` RPC returns all open streams with full details.
`getnetworkstats` RPC reports aggregate counts under `"streams"` key.

---

# 11 – Security

## pBFT: Honest Majority Assumption

Phase 1 uses pBFT with fixed committee size C = 137 and 2/3 Byzantine threshold:

| Parameter | Value |
|---|---|
| Committee | C = 137 (always, when Phase 1 active) |
| Threshold | T = ceil(137 × 2/3) = 92 votes required |
| Tolerated Byzantine nodes | 45 (137 − 92) |

Security is reinforced by VRF randomization: even with 30% of network controlled by attacker, the probability of 92 attackers landing in a random 137-committee is negligible. No fork needed — security is built-in from day one.

## Eclipse Attack Resistance

- IPv4: max 1 peer per /24 subnet
- IPv6: max 2 peers per /48 subnet

An attacker must control addresses from many different subnets.

## Sybil Resistance

- IPv6 subnet diversity enforcement (max 1 peer per /24 IPv4, 2 per /48 IPv6)
- No trusted setup, no staking requirement
- **Genesis Hash Guard:** prevents malicious or accidental parallel chains
  (see §6 — Genesis Node Chain Authentication)
- Heartbeat-chain: continuous liveness proof via BLAKE3-HMAC prevents
  ghost nodes from accumulating epoch-reward eligibility

The Genesis Hash Guard prevents chain multiplication; subnet diversity and
heartbeat continuity prevent identity multiplication at the P2P layer.

## Quantum Resistance: The 60-Second Window

Quantum computers pose a theoretical threat to ECDSA/Ed25519 signatures. Here is why Elektron Net remains secure without complex post-quantum cryptography:

### The Hash-Commitment Protection

```
UTXO stores:  BLAKE3(pubkey)[0:20]  ← only a hash, never the full key
Address is:   Bech32m("elek", BLAKE3(pubkey)[0:20])
```

As long as coins remain unspent, a quantum computer sees only a hash — it cannot reverse BLAKE3 to recover the private key. This is analogous to coins resting deep in your pocket: invisible to outside observers.

### The 60-Second Attack Window

When you spend coins, your public key is temporarily exposed in the broadcast transaction:

```
t = 0s:      Transaction broadcast — public key is now visible
t ≤ 60s:     Block producer includes transaction
t > 60s:     Block finalized — UTXO spent — quantum is too late
```

The window of vulnerability is exactly 60 seconds. For a quantum computer to successfully steal:

1. Intercept the transaction from the mempool
2. Compute private key from public key (Shor's algorithm)
3. Craft a contradictory transaction with higher fee
4. Propagate faster than the honest network

All within: **60 seconds**.

### The Realistic Threat Level

| Factor | Reality |
|--------|---------|
| Quantum computer exists today? | No — would need ~4,000 logical qubits, current systems have ~100-1,000 |
| Time until real threat | Realistically: 10-20+ years |
| 60-second window | Far too short for even future QC (error correction overhead) |
| Race against honest network | QC starts at a disadvantage — TX already propagating |

### Why No Zero-Knowledge Proofs

Zero-knowledge proofs would add quantum resistance but at a high cost:

- **Complexity:** New cryptographic constructions, setup ceremonies
- **Storage:** +200 bytes to +50KB per transaction
- **Speed:** Expensive verification, slower consensus
- **Simplicity:** Violates Elektron Net's design philosophy

Elektron Net uses a simpler approach: **the hash-commitment + 60-second rule**. As long as coins rest in the pocket (UTXO), they are quantum-safe. When spent, the window is too short for practical attack.

### Long-Term Migration

If a credible quantum threat emerges within the next decade, the network can hard-fork to post-quantum signatures (CRYSTALS-Dilithium, Falcon, or SPHINCS+). The 60-second protection window gives the network time to respond.

**Summary:**
- **Coins at rest:** Quantum-safe (only hash stored)
- **Coins in transit:** 60-second vulnerability window
- **After finalization:** Permanently safe, even against quantum
- **Long-term:** Hard-fork capability if needed

No ZKP. No ceremonies. No bloat. Just mathematics and the present moment.

## Quantum Security Analysis and Fixed Bitcoin Core Vulnerabilities

Elektron Net is a direct fork of Bitcoin Core. This section documents every
cryptographic primitive in the codebase, its quantum-attack surface, and the
concrete mitigation implemented in Elektron Net. It also records which Bitcoin
Core design decisions were improved upon and why.

### Threat Model

Two quantum algorithms apply to blockchain cryptography:

| Algorithm | Breaks | Speed-up |
|-----------|--------|----------|
| **Shor's algorithm** | ECDLP, RSA factoring | Polynomial — full break |
| **Grover's algorithm** | Symmetric keys, hash functions | Quadratic — halves effective bit-strength |

A cryptographically-relevant quantum computer (CRQC) with ~4,000 logical
qubits and sufficient gate fidelity would run Shor's algorithm. Today's
hardware has ~1,000 noisy physical qubits; the CRQC threat is realistic in
the 10-20 year range.

### HIGH — secp256k1 / ECDSA (inherited from Bitcoin Core)

**File:** `src/pubkey.h`, `src/key.h`, `src/script/interpreter.cpp`

**Risk:** Shor's algorithm solves the elliptic-curve discrete-log problem
(ECDLP) in polynomial time. Any node with a CRQC can derive any secp256k1
private key from the corresponding public key.

**Bitcoin Core exposure:**
- P2PK outputs publish the public key directly in the `scriptPubKey` — an
  attacker can harvest all P2PK keys from the chain today and decrypt them
  later when a CRQC becomes available.
- P2PKH/P2WPKH outputs expose the key at spend time, creating a 60-second
  window (the block interval) during which the key is visible in the mempool.

**Elektron Net mitigations:**
1. **Stealth addresses** (`src/wallet/stealth.cpp`) — every payment derives a
   fresh one-time Ed25519 key via DKSAP (Dual-Key Stealth Address Protocol).
   Even if a key is harvested, it corresponds to exactly one UTXO rather than
   an entire wallet.
2. **New output type** — stealth outputs use `<P[32]> OP_CHECKSIG_ED25519`
   (`src/script/interpreter.cpp`), a new 64-byte Ed25519 signature path that
   replaces secp256k1 for all freshly generated outputs.
3. **60-second finality** — pBFT finality within one block slot (60 s) gives
   attackers only a 60-second window once a public key is revealed in the
   mempool — too short for even a future CRQC.
4. **Phase 2 — implemented:** `OP_CHECKSIG_DILITHIUM` (`0xbe`) and
   `OP_CHECKSIGVERIFY_DILITHIUM` (`0xbf`) — CRYSTALS-Dilithium3 (NIST FIPS 204
   standard). Pubkey 1952 bytes, signature 3293 bytes. Enabled via
   `#ifdef HAVE_DILITHIUM` (compile with `-DWITH_DILITHIUM=ON`). Hash:
   `BLAKE3("ElektronNet/Dilithium3SigHash" || SHA256d_sighash)`.
   See `src/script/interpreter.cpp`.

### HIGH — Ed25519 (Elektron Net additions)

**Files:** `src/consensus/vrf.cpp`, `src/consensus/pbft.cpp`

**Risk:** Ed25519 is based on Curve25519 (twisted Edwards form), which is
still an ECDLP problem. Shor's algorithm breaks it identically to secp256k1.

**Elektron Net exposure:** VRF proofs and pBFT vote signatures reveal Ed25519
public keys.

**Mitigations:**
- **Epoch key rotation** — VRF keys rotate every 1,440 slots (24 hours).
  A harvested key is valid for at most one epoch.
- **Short-lived exposure** — keys appear in consensus messages, not in
  long-lived on-chain UTXOs. The time window for a quantum attack is bounded
  by the epoch length, not by UTXO age.
- **Future:** Replace the libsodium IETF ECVRF with a hash-based VRF
  (VOPRF-SHA3 or VOPRF-BLAKE3) once standardised.

### HIGH — x25519 / Curve25519 ECDH (Elektron Net additions)

**File:** `src/wallet/stealth.cpp`

**Risk:** The x25519 Diffie-Hellman used in stealth address derivation is
also an ECDLP problem. Shor's algorithm recovers the shared secret from the
ephemeral public key `R`.

**Mitigations:**
- **Per-transaction ephemeral keys** — each payment generates a fresh
  ephemeral scalar `r`. An attacker must run Shor's algorithm separately for
  every individual payment, not once per wallet.
- **Phase 2 — implemented:** Hybrid x25519 + CRYSTALS-Kyber-1024 KEM stealth
  addresses (`src/crypto/hybrid_kem.h/cpp`, `src/wallet/stealth.cpp`).
  `shared_secret = BLAKE3("ElektronNet/HybridKEM" || x25519_dh || kyber_dh)`.
  Bech32m HRP `"elek1pq"`. OP_RETURN payload = R_x25519[32] ‖ kyber_ct[1568]
  (1600 bytes total). Even if ECDLP is broken, the Kyber-1024 component
  remains secure under Module-LWE hardness (NIST FIPS 203).

### MEDIUM — RIPEMD-160 (Bitcoin Core design flaw, partially fixed)

**File:** `src/crypto/ripemd160.h`, `src/script/standard.cpp`

**Risk:** Bitcoin Core uses `Hash160 = RIPEMD160(SHA256(pk))` for P2PKH
addresses. RIPEMD-160 produces a 160-bit output; Grover's algorithm reduces
its quantum security to **80 bits** — classified as marginal by NIST.

**Bitcoin Core design flaw:** There was no technical need to use RIPEMD-160
when SHA-256 (256-bit output, 128-bit Grover security) was already in the
codebase. The choice was historical accident, not deliberate design.

**Elektron Net fix:**
- New wallets default to **bech32m** addresses (P2TR-style), which use only
  SHA-256 and BLAKE3. RIPEMD-160 is never invoked for newly generated
  addresses.
- P2PKH outputs are accepted for legacy compatibility but never generated
  by the Elektron Net wallet.
- The `src/crypto/quantum_notes.h` register tracks this as `[MEDIUM]`.
- **Phase 2 — implemented:** P2PKH output generation is deprecated in the
  wallet layer. `ParseOutputType("legacy")` logs a `BCLog::WALLET` deprecation
  warning; `getnewaddress` and `getrawchangeaddress` emit `WalletLogPrintf`
  warnings when `OutputType::LEGACY` is requested. P2PKH outputs are still
  *accepted* for legacy compatibility but are never generated by Elektron Net
  wallets. See `src/outputtype.cpp` and `src/wallet/rpc/addresses.cpp`.

### LOW — BLS12-381 Pairing (optional pBFT aggregate signatures)

**File:** `src/consensus/pbft.cpp` (guarded by `HAVE_BLST`)

**Risk:** BLS signatures use an elliptic-curve pairing over BLS12-381. Shor's
algorithm applies to the underlying group.

**Mitigations:**
- Ed25519 individual votes are verified before the BLS aggregate is checked.
  A compromised BLS aggregate alone cannot forge consensus.
- BLS is used only for signature aggregation efficiency, not as a sole trust
  anchor.
- **Future:** Replace with a lattice-based aggregate scheme (e.g., Falcon)
  once a standardised construction is available.

### SAFE — BLAKE3 (Elektron Net additions)

**Files:** `src/node/heartbeat.cpp`, `src/consensus/vrf.cpp`,
`src/script/interpreter.cpp` (sighash for Ed25519)

Grover's algorithm reduces 256-bit BLAKE3 security to **128 bits** — well
above the 112-bit threshold generally considered secure post-quantum. All
Elektron Net-specific hashing (heartbeat HMACs, VRF alpha derivation,
Ed25519 sighash domain separation) uses BLAKE3.

### SAFE — SHA-256 / SHA-256d (inherited from Bitcoin Core)

**File:** `src/crypto/sha256.h`

Grover's algorithm reduces to 128-bit security. Block and transaction
hashing remains secure against quantum adversaries. No change required.

### SAFE — 137-Day Pruning Window

**File:** `src/kernel/chainparams.cpp` (`nPruneAfterHeight = 197280`)

Older UTXOs are pruned from the UTXO set and chain history after 137 days.
This limits the public-key harvest window available to a future quantum
adversary: harvested keys from pruned history correspond to UTXOs that are
either already spent (worthless) or no longer traceable on the chain.

### Summary Table

| Component | Algorithm | Quantum Risk | Status |
|-----------|-----------|-------------|--------|
| secp256k1 / ECDSA | Shor | **HIGH** | Mitigated: stealth addr + OP_CHECKSIG_ED25519 |
| Ed25519 (VRF/pBFT) | Shor | **HIGH** | Mitigated: epoch key rotation |
| x25519 ECDH (stealth) | Shor | **HIGH** | ✅ Phase 2: hybrid x25519+Kyber-1024 KEM |
| RIPEMD-160 (Bitcoin Core) | Grover | **MEDIUM** (80-bit) | ✅ Phase 2: P2PKH generation deprecated |
| BLS12-381 (pBFT agg) | Shor | LOW | Mitigated: Ed25519 checked first |
| BLAKE3 | Grover | SAFE (128-bit) | No action needed |
| SHA-256 / SHA-256d | Grover | SAFE (128-bit) | No action needed |
| 137-day pruning | — | SAFE | Limits harvest window |
| Dilithium3 (Phase 2) | — | SAFE | NIST PQC standard (LWE hardness) |
| Kyber-1024 KEM (Phase 2) | — | SAFE | NIST PQC standard (Module-LWE) |

### Migration Roadmap

**Phase 1 (implemented):**
- Stealth addresses (DKSAP) reduce key exposure to one UTXO per payment
- `OP_CHECKSIG_ED25519` enables Ed25519 spending paths in the script engine
- bech32m default eliminates RIPEMD-160 for all new addresses
- BLAKE3 heartbeat HMACs provide quantum-safe liveness proofs
- 137-day pruning limits the quantum key-harvest window

**Phase 2 (implemented):**
- ✅ `OP_CHECKSIG_DILITHIUM` (`0xbe`) — CRYSTALS-Dilithium3 (NIST FIPS 204)
- ✅ Hybrid x25519 + Kyber-1024 KEM for stealth address DH (NIST FIPS 203)
- ✅ P2PKH output generation deprecated in wallet layer (log warning)
- ✅ Lepton Streams micropayment infrastructure
- ✅ BLS aggregate enforcement post-genesis (non-empty committee)
- ✅ PBFTAGG (`elkpbftagg`) P2P message for aggregate broadcast
- ✅ Developer API: `getnetworkstats`, `getpbftstatus`, `liststreams`, `getchannelstats`
- ✅ Bootstrap registry: `action=heartbeat` (30-min TTL refresh) + `action=stats`
- ✅ Bootstrap HTTPS/TLS support (`WITH_BOOTSTRAP_TLS=ON`)

**Phase 3 (long-term):**
- Hash-based VRF (VOPRF-BLAKE3) replacing ECVRF
- Lattice-based aggregate signatures replacing BLS12-381 in pBFT
- Full P2PKH spending ban after migration grace period

## VRF Sort-Order Security

VRF Selection is not a vulnerability.

**The VRF process is unbiasable:**
- Same input (slot, checkpoint_hash, utxo_root) → same output
- An attacker cannot manipulate their VRF output
- Cannot predict or influence other nodes' outputs

**Knowing your committee status is not an advantage:**
- Attacker knows if they are in the committee or not
- This provides no additional attack capability
- Disruption attempts are the same as in any PoS/PoW system

**The 2/3 Byzantine threshold protects against disruption:**
- Even with 30% of Full Nodes not participating
- The remaining 70% still provides strong finality
- Honest majority is mathematically guaranteed

## Channel Security: Block Producer as Watchtower

**Channel dispute window:** 36 blocks (36 minutes)

**Problem:** If a channel partner goes offline, they cannot contest a fraudulent old state broadcast.

**Solution:** Block Producer IS the Watchtower. Every Block Producer validates all transactions in the block. This validation includes checking channel close transactions:
1. Is this a channel close?
2. Is it the latest known state?
3. If not → Broadcast fraud proof immediately

**The 25% producer reward already covers this work:**
- Producer already validates ALL transactions
- Watchtower is a natural side-effect of block production
- No extra computation or storage required

**Fraud penalty:** If a node broadcasts an old channel state and gets caught, they lose their security deposit (2× the disputed amount). The Producer who caught the fraud earns a share of the penalty.

## Known Limitations

### Network Partition
pBFT chooses safety over liveness. During a partition, no finality is reached. On reconnect, the network heals automatically:
- Higher checkpoint height wins
- If equal height: lower checkpoint hash wins (deterministic)
- No manual intervention required.

---

# Part III – Getting Started

# 12 – Running Elektron Net

## Requirements

| Mode | CPU | RAM | Storage |
|---|---|---|---|
| Minimum | 2 | 4 GB | 10 GB |
| Full Node | 4+ | 8 GB | 50 GB SSD |
| Light/IoT Mode | — | 1 GB | 1 GB |

## Getting Started

**For any user** (send / receive Elek):
1. Download the software
2. Read and confirm the Data Sovereignty Declaration
3. Choose "Create New Wallet"
4. Write down your 24-word seed phrase (BIP-39, Qt GUI only)
5. Your wallet is active immediately — no block confirmation needed. Send and receive Elek, including stealth payments, right away.

**To earn epoch rewards** (every Full Node is eligible):

6. In Settings → activate an address for rewards ("Activate for Pool"). The wallet marks this address and sends `NODEREGISTER` automatically.
7. Keep your node running. Every epoch (~10 minutes) the node re-sends `NODEREGISTER` for your active address — this proves you are online. No extra steps needed.
8. VRF selects up to 21,000 online participants. If selected: reward auto-paid to your active address.

No balance required — anyone with zero Elek can participate from day one. Offline = node stops sending `NODEREGISTER` → not in the next draw.

**Full Node consensus participation** (earn the 25% producer share):

Steps 1–8 are the same as above (also in the 75% pool when online).

9. Keep node online continuously (heartbeats every 60 s).
10. After 137 days of recorded uptime → eligible for VRF committee.
11. Node can be selected as block producer → earns the 25% producer share when selected. (The 75% pool access is shared with all other nodes — no extra share.)

## The Data Sovereignty Declaration

Before any wallet is created, every user confirms this declaration. This is not a terms-of-service agreement. This is the user's explicit assertion of their fundamental rights – a statement of intent that is architectural, not contractual.

```
┌─────────────────────────────────────────────────────────────────┐
│                    DATA SOVEREIGNTY DECLARATION                   │
│                    Elektron Net – First Start                     │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  "I install this software with the explicit intent that my        │
│   transaction data and network activity are never permanently     │
│   stored – anywhere, by anyone, at any time.                    │
│                                                                  │
│   I understand that Elektron Net enforces this through automatic  │
│   deletion after exactly 137 days (α⁻¹). This deletion is:      │
│                                                                  │
│   → automatic    (no action required from me)                   │
│   → unconditional (applies to all nodes, no exceptions)           │
│   → irreversible  (mathematically enforced by protocol)          │
│                                                                  │
│   I exercise my fundamental right to privacy and my right to     │
│   be forgotten – not as a request to a company, but as an        │
│   architectural property of the software I am voluntarily        │
│   installing.                                                     │
│                                                                  │
│   I understand that the protocol enforces 137-day pruning        │
│   technically. No node can retain data beyond 137 days           │
│   without modifying the software, which would place them on       │
│   a separate network.                                            │
│                                                                  │
├─────────────────────────────────────────────────────────────────┤
│  LEGAL REFERENCES – UNIVERSAL PRIVACY RIGHTS                      │
│                                                                  │
│  GDPR           | EU General Data Protection Regulation           │
│  UDHR Art. 12   | Universal Declaration of Human Rights         │
│  CCPA           | California Consumer Privacy Act                 │
│  LGPD           | Brazil General Data Protection Law              │
│  UK GDPR        | United Kingdom Data Protection Act              │
│  PIPEDA         | Canada Personal Information Protection Act      │
│  APPI           | Japan Act on Protection of Personal Information│
│  DPDP Act       | India Digital Personal Data Protection Act      │
│  nDSG           | Switzerland Federal Act on Data Protection       │
│  Privacy Act    | Australia Privacy Act 1988                     │
│                                                                  │
│  This declaration asserts rights under the laws of my              │
│  jurisdiction, regardless of where the network operates.        │
│                                                                  │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  [ ] I have read and understood this declaration                 │
│                                                                  │
│                              [Continue →]                        │
└─────────────────────────────────────────────────────────────────┘
```

The checkbox cannot be pre-ticked. The Continue button is disabled until confirmed. There is no skip option.

## Roadmap

**Phase 1 — Foundation (Months 1–6)**
- `elektrond` daemon + CLI
- eGUI wallet (send, receive, balance)
- Genesis Mode fully operational (FN < 137)
- Stealth addresses + Bloom-Filter scanning
- Phase 1 consensus when N ≥ 137 (VRF + pBFT)
- ElektronChannels with built-in Watchtower (Producer validates)

**Phase 2 — Full Features (Months 7–12) ✅ Implemented**
- ✅ Lepton Streams (micropayments — `elkstrmopen` / `elkstrmtick` / `elkstrmclose`)
- ✅ `OP_CHECKSIG_DILITHIUM` post-quantum signature opcode (CRYSTALS-Dilithium3)
- ✅ Hybrid x25519 + Kyber-1024 KEM for stealth addresses
- ✅ P2PKH output deprecation in wallet
- ✅ BLS aggregate enforcement post-genesis
- ✅ `elkpbftagg` P2P message (pBFT aggregate broadcast)
- ✅ Developer API (`getnetworkstats`, `getpbftstatus`, `liststreams`, `getchannelstats`)
- ✅ Bootstrap registry heartbeat + stats + HTTPS/TLS support
- Mobile wallet (planned Phase 3)
- Block explorer (planned Phase 3)

**Phase 3 — Ecosystem (Year 2)**
- Exchange integrations
- Hardware wallet support
- Light client improvements

---

# Part IV – The Philosophy

# 13 – Closing Words

## What We Have Built

Elektron Net is a decentralised digital currency with anonymous payments by default, a fair reward system for every participant, and a network that is fully functional from the first second with a single node.

## Mathematical Properties

| Property | Guarantee |
|---|---|
| Your wallet belongs to you | Ed25519: theft mathematically infeasible |
| Your payments are private | Stealth addresses: linking infeasible |
| Your balance is pseudonymous | Addresses are hashes, amounts visible but unlinked |
| The supply is limited | 21,000,000 Elek, anchored in Block #0 |
| Your rewards are fair | 25% producer / 75% online |
| Your payment is fast | 60-second deterministic finality |
| Your history is forgotten | 137-day pruning, protocol-enforced |
| The network starts alone | Genesis Mode, one node is enough |
| The transition is automatic | Phase 1 at N = 137 |

These are not promises. They are mathematical and architectural properties. They do not depend on anyone keeping their word.

## From the Lydian Elektron to the Digital Elek

2,600 years. The same principle. New tools. Value through mathematics, not through decree.

Elektron Net is built on Stoic principles:

- **Freedom** – What is up to us (our keys, our choices)
- **Acceptance** – What is not up to us (market prices, adoption rates)
- **Responsibility** – Freedom without responsibility is noise

The mathematics secures your money. You decide how to use it.

---

# Appendix A – Glossary

Abbreviations and technical terms used in this document, sorted alphabetically.

---

## Currency Units

**Elek:** The base currency of Elektron Net. Maximum supply: 21,000,000 Elek. Divisible into 100,000,000 Lep. Symbol: Ⓔ (informal).

**Lep (Lepton):** The smallest on-chain unit. `1 Elek = 100,000,000 Lep`. Named after the lepton particle family (electrons, muons, tau). Corresponds to the satoshi in Bitcoin. Used for all on-chain amounts (`CAmount`).

**cLep (centi-Lepton):** Off-chain micropayment sub-unit. `1 Lep = 100 cLep`. Used exclusively in Lepton Streams to accumulate sub-Lep charges between settlements. Never appears on-chain.

---

## Elektron Net Concepts

**α⁻¹ (Alpha Inverse):** The fine-structure constant ≈ 1/137. Used throughout: 137-day pruning window, C_max = 137 (VRF committee size), MAX_HTLC_PER_CHANNEL = 137.

**BIP (Bitcoin Improvement Proposal):** Standardised design document for Bitcoin/Elektron Net protocol changes. Elektron Net adopts BIP-32 (HD derivation), BIP-39 (seed phrases), BIP-84 (P2WPKH paths), and BIP-21 (payment URIs).

**BLAKE3:** Fastest modern cryptographic hash function. Used for addresses, block hashes, Merkle roots, heartbeat HMAC, stream ID derivation, and checkpoint digests. Replaces SHA256d in all Elektron Net-specific constructions.

**BLS (Boneh–Lynn–Shacham):** Aggregate signature scheme over the BLS12-381 pairing-friendly elliptic curve. Multiple validator signatures are combined into one 96-byte aggregate. Used for pBFT finality and 137-day UTXO-root checkpoints.

**BLS12-381:** Pairing-friendly elliptic curve used by BLS signatures. Provides ~128-bit security.

**Bech32m:** Human-readable address encoding. Prefix `elek1` (standard), `elek1st` (stealth), `elek1pq` (hybrid post-quantum stealth). Error-detecting checksum.

**CCPA (California Consumer Privacy Act):** US state privacy law. Elektron Net's 137-day structural pruning ensures compliance by design.

**Checkpoint:** A BLS-signed UTXO root hash produced every 137 days. Allows new nodes to bootstrap from a cryptographically verified recent state.

**CLTV (CheckLockTimeVerify):** Bitcoin/Elektron script opcode enforcing an absolute block-height lock. Used in HTLC expiry conditions.

**CRQC (Cryptographically Relevant Quantum Computer):** A quantum computer powerful enough to break ECDLP (rendering secp256k1 and Ed25519 insecure). Elektron Net's hybrid post-quantum layer is designed to remain secure against CRQCs.

**CRYSTALS (Cryptographic Suite for Algebraic Lattices):** NIST-standardised suite of post-quantum algorithms. Elektron Net uses two CRYSTALS schemes: Dilithium (signatures) and Kyber (key encapsulation).

**CSV (CheckSequenceVerify):** Bitcoin/Elektron script opcode enforcing a relative time-lock. Used in channel revocation and dispute windows.

**Dilithium3 / CRYSTALS-Dilithium:** Lattice-based post-quantum digital signature algorithm, standardised as NIST FIPS 204. Pubkey: 1952 bytes, signature: 3293 bytes. Opcode: `OP_CHECKSIG_DILITHIUM (0xbe)`.

**DST (Domain Separation Tag):** A unique ASCII prefix prepended before hashing to prevent cross-protocol collision attacks. Example: `"ELEKTRON_PBFT_BLS_SIG_v1"`.

**ECDH (Elliptic Curve Diffie-Hellman):** Key-exchange protocol. Used in stealth address scanning (sender derives one-time key from recipient's scan pubkey).

**ECDLP (Elliptic Curve Discrete Logarithm Problem):** The mathematical hardness assumption underlying ECDSA and Ed25519. Solvable by Shor's algorithm on a CRQC.

**Ed25519:** Twisted Edwards curve digital signature algorithm. Fast, deterministic, 64-byte signatures. Default signature scheme in Elektron Net for P2P messages, channel updates, and stream authentication.

**eGUI:** Elektron Net's Qt-based desktop wallet GUI (`elektron-qt`). Provides address management, transaction history, stream controls, and the Data Sovereignty Declaration dialog.

**elektron-cli / elektrond:** Command-line interface and daemon binary for Elektron Net.

**ElektronChannels:** Bi-directional payment channels secured by on-chain multi-sig scripts with HTLC and CSV/CLTV time-lock conditions. Analogous to Lightning Network channels but using BLAKE3/Ed25519 natively.

**elkhb:** P2P message type. `elkhb` — heartbeat liveness proof (72 bytes: node_id[32] ‖ slot[8] ‖ hmac_hash[32]).

**elkreg / elkuser:** P2P message types. `elkreg` — Full Node registration. `elkuser` — lightweight user registration.

**elkchopen / elkchclose:** P2P message types. `elkchopen` — open an ElektronChannel. `elkchclose` — close with final state.

**elkstrmopen / elkstrmtick / elkstrmclose:** P2P message types for Lepton Streams. Open, metering tick, and close messages.

**elkpbftagg (PBFTAGG):** P2P message broadcasting a pBFT BLS aggregate signature after COMMIT quorum (147 bytes). Signals block finality to the network.

**FN (Full Node):** A node that stores the full chain, validates all blocks, participates in VRF committee selection, and sends heartbeats. Requires ≥ 137 days of continuous operation to be eligible.

**GDPR / DSGVO (General Data Protection Regulation / Datenschutz-Grundverordnung):** EU data protection law. Elektron Net satisfies GDPR Art. 5(1)(e) (storage limitation) and Art. 17 (right to erasure) structurally — transaction data is deleted after 137 days by protocol design, not policy.

**Genesis Block:** The first block in the chain. Its hash uniquely identifies the Elektron Net network and is hardcoded in all clients.

**Genesis Mode:** Consensus phase active when fewer than 137 Full Nodes are online. Single-node block production without pBFT quorum. Transitions to Phase 1 once ≥ 137 FNs are registered.

**Grover's Algorithm:** Quantum algorithm that provides a quadratic speedup for searching unsorted databases. Halves the effective security of symmetric-key algorithms (AES-128 → 64-bit effective). Does not break ECDLP.

**HD (Hierarchical Deterministic):** Wallet key derivation standard (BIP-32). All Elektron Net keys are derived from a single seed phrase via path `m/137'/0'/N`.

**HMAC (Hash-based Message Authentication Code):** Message authentication code using a keyed hash. Elektron Net uses BLAKE3-HMAC for heartbeat authentication: `BLAKE3(epoch_secret ‖ node_id ‖ slot_le)`.

**HRP (Human-Readable Part):** The prefix of a Bech32m address string. Elektron Net HRPs: `elek1` (standard), `elek1st` (stealth x25519), `elek1pq` (hybrid Kyber stealth).

**HTLC (Hash Time-Locked Contract):** A conditional payment script: funds are released if the recipient provides a preimage R where `BLAKE3(R) = H`, before a CLTV deadline. Used for atomic multi-hop routing in ElektronChannels.

**KEM (Key Encapsulation Mechanism):** An asymmetric protocol for securely establishing a shared symmetric key. Elektron Net uses Kyber-1024 KEM for post-quantum stealth address key derivation.

**Kyber-1024 / CRYSTALS-Kyber:** Lattice-based post-quantum KEM, standardised as NIST FIPS 203. Provides IND-CCA2 security based on Module-LWE hardness. Used in hybrid stealth addresses alongside x25519.

**kvB (kilo-virtual-Bytes):** Fee rate unit: 1 kvB = 1000 virtual bytes. A typical 250-byte transaction is 0.25 kvB. Inherited from Bitcoin's fee denominator.

**Lepton Streams:** Continuous micropayment infrastructure built on ElektronChannels. Three billing models: `PER_SECOND` (video/voice), `PER_KB` (data transfer), `PER_REQUEST` (API calls). Charges accumulate in cLep off-chain; whole Lep are periodically settled into the channel balance.

**LGPD (Lei Geral de Proteção de Dados):** Brazil's data protection law. Structurally satisfied by Elektron Net's 137-day pruning.

**LWE / Module-LWE (Learning With Errors):** Computational hardness problem underlying CRYSTALS algorithms. Conjectured to be secure against quantum computers.

**Merkle Tree / Merkle Root:** A binary hash tree where each leaf is a transaction hash and each node is the hash of its children. The root is included in every block header for efficient SPV proofs.

**NIST (National Institute of Standards and Technology):** US federal agency that standardises cryptographic algorithms. FIPS 203 (Kyber), FIPS 204 (Dilithium) are NIST post-quantum standards used by Elektron Net.

**P2PKH (Pay-to-Public-Key-Hash):** Legacy Bitcoin address format (`1...`). Deprecated in Elektron Net — wallets never generate P2PKH outputs; only accept them for backwards compatibility.

**P2WPKH (Pay-to-Witness-Public-Key-Hash):** SegWit native address format. Default Elektron Net address type, encoded as Bech32m `elek1...`.

**pBFT (Practical Byzantine Fault Tolerance):** Consensus protocol tolerating up to 1/3 faulty nodes. Finality after ≥ 92 of 137 committee validators sign (PREPARE + COMMIT phases). Guarantees no forks once a block is finalised.

**PBFTAGG:** See `elkpbftagg`.

**ppm (Parts Per Million):** Unit for the max-fee-ratio cap. `10 000 ppm = 1%`. A fee capped at 10 000 ppm means it can never exceed 1% of the primary output value.

**RPC (Remote Procedure Call):** Elektron Net exposes a JSON-RPC 2.0 API (port 9337) for wallet and node management. Elektron-specific commands are in category `"elektron"`.

**Shor's Algorithm:** Quantum algorithm that solves ECDLP and integer factorisation in polynomial time. Breaks ECDSA, Ed25519, and RSA on a CRQC. Mitigated in Elektron Net by the CRYSTALS post-quantum layer.

**Stealth Address:** A one-time address derived by the sender from the recipient's published scan and spend public keys. Each payment lands at a unique address that cannot be linked to the recipient's identity or to other payments.

**UTXO (Unspent Transaction Output):** The fundamental unit of balance. Your wallet's funds are the sum of UTXOs you can unlock. Elektron Net prunes UTXOs older than 137 days (spent outputs only; unspent UTXOs are preserved).

**VRF (Verifiable Random Function):** Cryptographic function producing an output that is both unpredictable and verifiably correct. Used to select the 137-member pBFT committee and the block proposer for each slot without central coordination.

**Watchtower:** A monitoring service (built into every FN) that watches for stale-state channel close attempts. If a counterparty broadcasts an old commitment, the Watchtower detects it within the 36-block dispute window and triggers the fraud-penalty path.

**x25519:** Elliptic Curve Diffie-Hellman using Curve25519 in Montgomery form. Used for the classical (non-quantum) component of hybrid stealth address key agreement.

---

# Appendix B – Technical Parameters

## Consensus

| Parameter | Value |
|-----------|-------|
| Block Time | 60 seconds |
| Production Window | 45 seconds |
| Committee Size | 137 (fixed in Phase 1) |
| Byzantine Threshold | 2/3 = 92 votes |
| Phase 1 Start | FN ≥ 137 |
| Phase 1 End | FN < 137 → Genesis Mode |
| Dispute Window | 36 blocks |

## Economy

| Parameter | Value |
|-----------|-------|
| Max Supply | 21,000,000 Elek |
| Genesis Reward | 5 Elek |
| Halving | 4 years |
| Producer Share | 25% (instant, every block) |
| Online Pool | 75% (epoch end, every 10 min) |
| Epoch Length | 10 blocks (10 minutes) |
| Node Cap | 21,000 |
| Online Window | K = 10 slots |
| Min Relay Fee Floor | 100 cLep/kvB (1 Lep/kvB) — configurable via `-elektronfeefloor` |
| Max Fee Ratio Cap | 1% of primary output (10 000 ppm) — configurable via `-elektronmaxfeeratio` |

## Privacy

| Parameter | Value |
|-----------|-------|
| Pruning | 137 days |
| Address Format | Bech32m (elek1) |
| Key Derivation | BIP-32 m/137'/0'/N |

---

*This is the final form of the protocol. No promises. Only mathematics.*

**License: MIT**
