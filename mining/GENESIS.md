# Elektron Net Genesis Block Guide

This guide explains how to mine the Elektron Net genesis blocks, verify your private key, and integrate the results into the C++ source code.

---

## Overview

The `mine_genesis.py` script mines the very first block (genesis) for every Elektron Net network. It is a one-time operation performed before the chain goes live. After mining, the script produces a complete results file (`genesis_results.txt`) containing:

- Your **wallet credentials** (private key, WIF, public key, addresses)
- **Genesis block parameters** for all networks
- **Ready-to-paste C++ code** for `src/kernel/chainparams.cpp`

---

## Difficulty & Design Goal

Elektron Net targets a **60-second block time** and is designed so that a normal CPU can mine the genesis block in roughly **1 minute**.

| Parameter | Bitcoin | Elektron Net |
|-----------|---------|--------------|
| Block time | 10 minutes | **60 seconds** |
| powLimit | `0x00000000ffff...` | **`0x007fffff0000...`** (CPU-friendly) |
| nBits (genesis) | `0x1d00ffff` | **`0x1d7fffff`** (~200x easier) |
| Genesis reward | 50 BTC | **5 ELEK** |
| Smallest unit | satoshi | **Lepton** |

The difficulty will automatically adjust after the first 2,016 blocks (roughly 1.4 days) to maintain the 60-second target.

---

## Prerequisites

- Python 3.8+ (stdlib only — no external packages)
- A hex-encoded secp256k1 **private key** (64 characters) that you generated securely
- ~5 minutes of time

If you do not have a key yet, generate one first:

```bash
python generate_address.py --count 1 --output genesis_key.txt
```

Keep that file secure.

---

## Running the Genesis Miner

```bash
cd mining
python mine_genesis.py
```

The script will:

1. Ask you to **paste your private key** (64 hex chars).
2. **Verify** that the private key matches the hardcoded genesis public key.
3. Mine the genesis blocks for:
   - **Mainnet**
   - **Testnet3**
   - **Testnet4**
   - **Signet**
4. Write everything to `genesis_results.txt`.

### Example session

```
======================================================================
ELEKTRON NET GENESIS MINER
======================================================================

Please enter the genesis private key (64 hex characters).
This key controls the 5 Elek genesis reward.
Private key (hex): 95402f1dffe959ef95c0c403341e610e85d31bb7adf06ff7fe29ea6e142c30f7
Private key verified successfully.

Genesis pubkey: 0417256d59...5578479de3
P2WPKH address: be1qv7c77khv5kx33seahn8edfnfmxprxx6uce9pvc

Mining with nBits=0x1d7fffff (target ~200x easier than Bitcoin)
Reward = 5 Elek = 500,000,000 Lepton

=== Mining Mainnet genesis block ===
Merkle root: 025b2a1e...
Target:      007fffff00000000000000000000000000000000000000000000000000000000
  tried 1,000,000 nonces... 12,345,678 H/s
FOUND! nonce=123456789  hash=0000000a1b2c3d4e...  time=45.12s

=== Results ===
Mainnet:   time=1746456789 nonce=123456789 hash=... merkle=...
Testnet3:  time=1746456789 nonce=987654321 hash=... merkle=...
Testnet4:  time=1746456789 nonce=111111111 hash=... merkle=...
Signet:    time=1746456789 nonce=222222222 hash=... merkle=...

Results written to: C:\...\elektron-net\mining\genesis_results.txt
```

---

## After Mining

### 1. Open the results file

```bash
cat genesis_results.txt
```

This file contains four ready-to-paste C++ code blocks.

### 2. Paste into `src/kernel/chainparams.cpp`

Replace the placeholder (commented-out) lines in four locations:

| Network | Approx. line |
|---------|---------------|
| Mainnet | ~122 |
| Testnet3 | ~238 |
| Testnet4 | ~340 |
| Signet | ~478 |

Simply copy each block from `genesis_results.txt` and overwrite the matching commented section in `chainparams.cpp`.

### 3. Build

```bash
cd build
cmake --build .
```

### 4. Run the node

```bash
./src/elektrond -printtoconsole
```

If compilation succeeds and the node starts, your genesis parameters are correct.

---

## Key Files

| File | Purpose |
|------|---------|
| `mine_genesis.py` | The genesis mining script |
| `genesis_results.txt` | **All results + wallet + C++ code** |
| `generate_address.py` | Standalone address/key generator |
| `genesis_wallet.txt` | Backup of the genesis wallet data (manual) |

---

## Security Warning

`genesis_results.txt` contains the **private key** that controls the 5 ELEK genesis reward. This key is permanently embedded in the genesis block and cannot be changed without forking the chain.

- **Keep the file offline** if possible.
- **Never commit it to git.**
- **Delete it** after you have imported the key into a secure wallet.

To import into the Elektron Net wallet later:

```bash
./src/elektron-cli importprivkey "<WIF>"
```

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| "Private key does not match genesis pubkey" | You entered the wrong key. Use the one you generated with `generate_address.py` or the one that corresponds to the hardcoded `GENESIS_PUBKEY_HEX` in `mine_genesis.py`. |
| Mining takes >5 minutes | This is normal on slow CPUs. The target is designed for ~1 minute on a modern CPU; older machines may take longer. |
| Node fails `assert(hashGenesisBlock)` | You forgot to paste the new values from `genesis_results.txt` into `chainparams.cpp`. |

---

## Technical Notes

- **nBits `0x1d7fffff`** is a very easy difficulty, chosen so that CPU mining is viable at launch.
- After 2,016 blocks (~1.4 days), the difficulty retargets automatically to maintain the 60-second block time.
- **Testnet4** uses a different genesis message and a zero-pubkey output, consistent with Bitcoin Testnet4's design.
- **Regtest** does not need mining; its genesis block uses the minimum difficulty `0x207fffff` and is solved instantly.
- **Distinct genesis hashes:** Mainnet, Testnet3, Testnet4 and Signet each receive a different block timestamp, offset by 137 seconds between networks. This guarantees every chain has a unique genesis hash so nodes can never accidentally peer with the wrong network.
