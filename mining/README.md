# Elektron Net Standalone Mining Software

This directory contains standalone CPU mining tools for Elektron Net, fully conforming to the standard Bitcoin mining protocol (`getblocktemplate` + `submitblock`).

## Files

| File | Description |
|------|-------------|
| `mine_genesis.py` | Python script to mine genesis blocks for all networks. |
| `generate_address.py` | Generate Elektron Net addresses (P2PKH + P2WPKH) with private keys. |
| `miner.py` | Standalone Python miner. Connects via RPC, fetches templates, mines, submits. |
| `miner.cpp` | Standalone C++ miner. Multi-threaded, configurable via JSON. |
| `config.json` | Configuration for the C++ miner (RPC, threads, pool). |
| `CMakeLists.txt` | Build file for the C++ miner. |

## Protocol Compliance

Both miners use the **standard Bitcoin RPC mining protocol**:

1. `getblocktemplate` — fetch work from the node.
2. SHA-256d brute-force on the block header.
3. `submitblock` — send the solved block back to the node.

This is the same mechanism used by `cgminer`, `bfgminer`, and Bitcoin Core's internal `generate` (now `generatetoaddress`).

## Python Miner

### Requirements
- Python 3.8+
- No external dependencies (stdlib only)

### Usage

```bash
# With default RPC settings (localhost:8332, user/password)
python miner.py

# Custom RPC
python miner.py --url http://127.0.0.1:8332 --user elek --password secret --threads 8 --continuous
```

## C++ Miner

### Requirements
- C++20 compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.16+
- libcurl
- OpenSSL

### Build

```bash
cd mining
mkdir build && cd build
cmake ..
cmake --build .
```

### Usage

```bash
# Default config.json in same directory
./elektron_miner

# Custom config path
./elektron_miner /path/to/config.json
```

### Config File (`config.json`)

```json
{
  "rpc": {
    "url": "http://127.0.0.1:8332",
    "user": "user",
    "password": "password"
  },
  "mining": {
    "address": "be1q...",
    "threads": 4,
    "target_spacing": 60
  },
  "pool": {
    "enabled": false,
    "url": "stratum+tcp://pool.elektron-net.org:3333",
    "user": "worker.1",
    "password": "x"
  }
}
```

## Address Generator

Generate wallet addresses with full credentials (private key, WIF, public key, P2PKH, P2WPKH).

```bash
# Generate one random address
python3 generate_address.py

# Generate 5 addresses for a mining farm
python3 generate_address.py --count 5 --output my_farm.txt

# Deterministic generation from a seed
python3 generate_address.py --seed 95402f1dffe959ef95c0c403341e610e85d31bb7adf06ff7fe29ea6e142c30f7
```

Output is written to a `.txt` file containing all keys. **Keep it secure.**

## Genesis Mining

**Do NOT run `mine_genesis.py` until you are ready to finalize the chain parameters.**

When the time comes:

```bash
cd mining
python3 mine_genesis.py
```

This will output nonces, block hashes, and merkle roots for Mainnet, Testnet3, Testnet4, and Signet. Copy these values into `src/kernel/chainparams.cpp`.
