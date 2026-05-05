# Elektron Net 2.0 — Protocol Whitepaper
**Version 3.0 | Author: Ali Kutlusoy | Graz, Austria, 2026**

---

## Abstract

Elektron Net 2.0 is a minimal, focused fork of Bitcoin Core. It preserves Bitcoin's proven proof-of-work consensus, emission schedule, and network architecture in their entirety. Two deliberate protocol changes are introduced:

1. **Block time reduced to 60 seconds** — faster confirmation latency without altering the economic model.
2. **Mandatory 137-day pruning** — transaction history is mathematically erased after α⁻¹ days, leaving only headers, checkpoints, and the current UTXO set.

Everything else remains Bitcoin Core. The SHA-256d proof-of-work, the 21-million supply cap, the halving rhythm, and the Nakamoto consensus are untouched.

---

## 1 — Why

### The Lydian Elektron

Around 600 BC, the Lydians in Asia Minor struck the first coins in the world. They were made of Elektron – a naturally occurring alloy of gold and silver. These coins fundamentally changed civilisation: for the first time, a person could trade without giving their name. Value became transferable, divisible, anonymous.

The Lydian Elektron coins were the first decentralised currency in history. No king had to be present. No scribe had to record the exchange. The value lay in the material itself – in the mathematics of the alloy.

2,600 years later, the world faces the same problem, solved with the same means: mathematics instead of trust.

### 1.0 Philosophy & Stoic Stance

The Stoic masters — Seneca, Epictetus, Marcus Aurelius — taught a single, devastating discipline: *distinguish between what is in your power and what is not*.

Your thoughts. Your keys. Your next breath. These are yours.

The opinions of strangers. The records of yesterday. The judgment of history. These are not.

Bitcoin’s ledger is a monument to the past. Every transaction, every mistake, every late-night payment is etched in cryptographic stone — forever. That immutability is magnificent, but it is also a chain. A chain that grows heavier with every block. A chain that binds you to a history you never chose to carry.

Elektron Net rejects that chain. We do not deny history; we simply refuse to be its prisoner. The network remembers only what is necessary for consensus — the UTXO set, the headers, the checkpoints. The rest is released. Not hidden behind encryption. Not buried in fine print. Erased. Gone. As if it never was.

This is the **Stoic protocol**: secure what you can control, let go of what you cannot.

> *"You cannot stop the wind, but you can adjust your sails. You cannot erase the world’s memory, but you can build a network that forgets by design."*

### 1.1 The Pocket Philosophy

Elektron Net is deliberately built around a simple, ancient human experience: When you put on your pants in the morning and reach into your pocket, you immediately know three things:

- How much money you have right now — not last week, not last month.
- That this money is yours — because it is your pocket.
- You do not need to remember where each coin came from, what you bought yesterday, or who paid you last year. The past is irrelevant. Only the present matters.

This everyday reality is the guiding metaphor for the entire protocol.

Your UTXO set is your pocket. It contains exactly what you possess in this moment. Nothing more, nothing less. It is the only permanent data structure in the network.

137 days of pruning is natural forgetting. Just as you do not keep receipts from five months ago in your pocket, the network mathematically erases all transaction history after exactly 137 days (α⁻¹). Not because someone ordered it, but because the protocol makes it impossible to keep. No subpoenas, no archives, no eternal digital footprint.

- **Recovery is finding your pocket again.** Your 24-word seed is enough. Scan the current UTXO set, recognise what belongs to you, and reclaim it — no history required.

This is not merely privacy engineering. It is the **exercise of the worldwide right to be forgotten, by design**. No administrator can override it. No majority vote can extend it. The will of the user is encoded in consensus: after 137 days, the data is gone. Not because a company decided to comply, but because mathematics and physics demand it.

> *"Mathematics secures your money. Time erases your traces. You own the moment."*

### 1.2 History & The Problem

In 2009, Bitcoin emerged from the wreckage of institutional failure — a way to move value across the world without asking permission, without signing papers, without trusting a bank. Its invention of the blockchain created something unprecedented: an eternal, auditable, uncensorable record. In those early years, that permanence felt like liberation.

But permanence has a shadow.

As the chain grew, so did the surveillance surface. Chain-analysis firms mapped every wallet. State actors traced every flow. Data brokers packaged your financial life into sellable profiles. The very transparency that made Bitcoin trustworthy became the cage that made its users transparent. What began as liberation from banks quietly became, for millions, a panopticon they could never leave.

The European Union recognized this tension and codified the **Right to be Forgotten** in the GDPR. Other jurisdictions followed. Yet on a public blockchain, legal rights are paper tigers without technical enforcement. A court can order a search engine to delist a link; no court on Earth can order a million distributed nodes to forget a block they were designed to keep forever.

Elektron Net solves this at the root.

The 137-day pruning window is not a setting you can toggle. It is a structural guarantee that the network *cannot* retain what it has been ordered to forget — because the protocol itself makes retention impossible. Not difficult. Not illegal. Impossible.

An immutable global ledger of all financial history creates three inevitable catastrophes:

- **Permanent surveillance surface** — every payment you ever made is traceable forever, by anyone with a node and a database.
- **Unbounded storage growth** — nodes must carry the full weight of decades of data, pricing out ordinary users and centralising power.
- **Institutional risk** — data that exists can be subpoenaed, analysed, weaponised, and sold. If it is stored, it will be exploited.

Bitcoin was built for institutional resilience. Elektron Net 2.0 is built for human sovereignty — and for the ancient, inalienable right of every person to be forgotten.

---

## 2 — Technical Foundation

**Base:** Bitcoin Core (C++20 fork). All P2P, storage, script, wallet, and consensus infrastructure is inherited directly.

| Component | Source | Note |
|---|---|---|
| Language | Bitcoin Core | C++20 |
| Consensus | Bitcoin Core | SHA-256d PoW, Nakamoto longest-chain rule |
| Difficulty Adjustment | Bitcoin Core | Every 2,016 blocks (retargeting period halved in calendar time) |
| Script Engine | Bitcoin Core | OP_CODES unchanged |
| P2P Network | Bitcoin Core | TCP/IP, addr relay, DoS protection |
| Wallet | Bitcoin Core | BIP-32/39/44, descriptor wallets, Bech32m |
| Storage | Bitcoin Core | LevelDB / RocksDB |
| RPC | Bitcoin Core | Port 9337 (adapted) |

**Changes to Bitcoin Core:**

| Parameter | Bitcoin | Elektron Net 2.0 |
|---|---|---|
| Block time | 10 minutes | **60 seconds** |
| Blocks per day | 144 | **1,440** |
| Retarget interval | 2,016 blocks (2 weeks) | **2,016 blocks (1.4 days)** |
| Pruning | Optional, user-defined | **Mandatory, 137 days** |

All other consensus rules, opcodes, signature schemes (ECDSA, Schnorr/Taproot), and network behaviour are preserved exactly.

---

## 3 — The 60-Second Block

### 3.1 Rationale

A 10-minute interval was conservative in 2009. In 2026, network propagation, hardware, and bandwidth make 60-second blocks practical without materially increasing orphan rates, provided difficulty retargeting remains responsive.

### 3.2 Economic Preservation

Reducing block time by 10× would inflate supply by 10× if the block reward were unchanged. The emission schedule is therefore scaled proportionally:

| Parameter | Value |
|---|---|
| Block time | 60 seconds |
| Blocks per day | 1,440 |
| Genesis block reward | **5.00 Elek** |
| Halving interval | 2,102,400 blocks (4 years) |
| Maximum supply | **21,000,000 Elek** |

| Period | Block Reward | Per Day | Cumulative |
|---|---|---|---|
| Year 1–4 | 5.00 Elek | 7,200 | 10,512,000 |
| Year 5–8 | 2.50 Elek | 3,600 | 15,768,000 |
| Year 9–12 | 1.25 Elek | 1,800 | 18,396,000 |
| … | … | … | … |
| ~Year 115 | ≈0.00000 | 0 | 21,000,000 |

**No pre-mine. No airdrop. No founder allocation.** Every Elek is earned by producing valid proof-of-work.

---

## 4 — 137-Day Pruning: The Forgetting

### 4.1 Core Principle

Elektron Net does not store transaction history indefinitely. Full block data (transactions, inputs, outputs) is deleted after exactly 137 days — α⁻¹, the inverse fine-structure constant.

This is structural impossibility, not policy:
- Every node deletes simultaneously — no node can retain data without forking onto a separate chain.
- No company can be subpoenaed for records that no longer exist.
- No hacker can breach a database that has been mathematically erased.

The 137-day window enforces the **right to be forgotten** as a protocol invariant. It is not a service offered to users; it is a property of the network demanded by them. The user’s will is not expressed through a privacy toggle or a terms-of-service checkbox. It is expressed through hash power, through node operation, through the consensus rules themselves. To run Elektron Net is to vote for forgetting. To mine Elektron Net is to execute that vote.

### 4.2 What Each Node Stores

| Retention | Data |
|---|---|
| **Permanent** | Genesis block header (80 bytes) |
| | All block headers (80 bytes each, chain integrity) |
| | UTXO set (current unspent outputs) |
| | Checkpoints (BLS or threshold-signed, one per pruning window) |
| **137 days, then deleted** | Full transaction content |
| | Input/output scripts and amounts |
| **Never stored** | User identity, IP mappings, transaction graphs |

### 4.3 Checkpoint Trust Model

After 137 days, history is gone. New nodes need a way to trust the UTXO set.

**Checkpoints:**
- Produced every 137 days (197,280 blocks at 60s).
- Contains: block height + UTXO Merkle root.
- Signed by a threshold of recent miners (adapted from Bitcoin Core assumevalid logic).

**Bootstrap for a new node:**
1. Download genesis header.
2. Download latest checkpoint; verify UTXO root.
3. Download UTXO set from multiple peers; compare root.
4. If roots match: chain state is verified. No full sync of 137+ days of transactions required.

### 4.4 Storage Projection

| Component | Year 1 | Year 5 | Year 10 |
|---|---|---|---|
| UTXO Set | ~50 MB | ~1–2 GB | ~3–6 GB |
| Block Headers | <1 MB | ~12 MB | ~24 MB |
| Checkpoints | <1 MB | <1 MB | <1 MB |
| **Total permanent storage** | **~100 MB** | **~2–3 GB** | **~5–8 GB** |

Compared to Bitcoin's unbounded growth, this is a **~100× reduction** after 10 years.

### 4.5 Wallet Recovery Without History

1. Enter 24-word BIP-39 seed.
2. Derive all wallet descriptors (BIP-44/84/86).
3. Download the current UTXO set from peers.
4. Scan for outputs matching your derived addresses.
5. Wallet is fully functional — balance spendable, no history needed.

This works because the UTXO set *is* the pocket. The past is irrelevant.

---

## 5 — Quantum Security

Elektron Net 2.0 does not introduce novel post-quantum primitives. Instead, it amplifies the protections already present in Bitcoin's design and accelerates them through the 60-second finality window.

### 5.1 Hash-Commitment at Rest

Standard Bech32m / P2WPKH / P2TR addresses publish only a hash of the public key. A quantum computer cannot reverse BLAKE3 or SHA-256 to recover the key. Coins that have not been spent are **quantum-safe at rest**.

### 5.2 The 60-Second Exposure Window

When a transaction is broadcast, the public key appears in the mempool:

```
t = 0s:   Transaction broadcast — public key visible.
t ≤ 60s:  Miner includes transaction in block.
t > 60s:  Block confirmed — UTXO spent — quantum computer is too late.
```

To steal funds, a quantum attacker would need to:
1. Intercept the transaction from the mempool.
2. Run Shor's algorithm to derive the private key.
3. Craft a conflicting transaction with a higher fee.
4. Out-propagate the honest network.

All within **60 seconds**. Error-correction overhead for a cryptographically-relevant quantum computer makes this window effectively unexploitable.

### 5.3 Pruning as a Harvest Limit

After 137 days, all spent outputs and their revealed public keys are pruned. A future quantum adversary cannot harvest historical keys from the chain. The attack surface is limited to the current mempool window.

### 5.4 Future-Proofing

If a credible quantum threat emerges, the network can soft-fork to post-quantum signature schemes (e.g., CRYSTALS-Dilithium, Falcon, SPHINCS+) through Bitcoin's established upgrade mechanisms. No hardcoded cryptography is changed today; the protocol retains full upgrade flexibility.

---

## 6 — Network Parameters

| Category | Parameter | Value |
|---|---|---|
| **Consensus** | Algorithm | SHA-256d Proof-of-Work |
| | Block time | 60 seconds |
| | Difficulty retarget | Every 2,016 blocks |
| | Finality | Probabilistic (Nakamoto) |
| **Economy** | Max supply | 21,000,000 Elek |
| | Genesis reward | 5.00 Elek |
| | Halving interval | 2,102,400 blocks (4 years) |
| | Fee model | Market-based, 100 % to miner |
| **Privacy** | Pruning window | 137 days (197,280 blocks) |
| | Address format | Bech32m (`be1q...` / `be1p...`) |
| | Default output type | P2WPKH / P2TR (Taproot) |
| **Node** | Minimum storage | ~100 MB (Year 1) |
| | Full node storage | ~5–8 GB (Year 10) |
| | RAM (full node) | 4–8 GB |

---

## 7 — Conclusion

Elektron Net 2.0 is Bitcoin Core with two surgical modifications:

1. **60-second blocks** — faster payments, preserved economics.
2. **137-day pruning** — privacy by mathematics, storage bounded forever.

No new consensus mechanisms. No new trust models. No administrator keys. No pre-mine.

The same SHA-256d proof-of-work. The same 21-million cap. The same Nakamoto consensus.

> *"Mathematics secures your money. Time erases your traces. You own the moment."*

---

**License: MIT**