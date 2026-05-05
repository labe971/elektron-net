#!/usr/bin/env python3
"""
Mine new genesis blocks for Elektron Net.

Targets:
- Mainnet / Testnet3 / Signet: same genesis tx (Elektron message + pubkey)
- Testnet4: different message + zero pubkey
- Regtest: not needed (already very easy powLimit)

New parameters:
- nBits = 0x1d0a0000  (10x easier than Bitcoin's 0x1d00ffff)
- Reward = 5 * COIN = 500_000_000 Lepton

After running, check genesis_results.txt for all values needed for
src/kernel/chainparams.cpp and your wallet credentials.
"""

import hashlib
import os
import struct
import sys
import time

# =============================================================================
# secp256k1 helpers for pubkey verification
# =============================================================================
P = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
N = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141
GX = 0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798
GY = 0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8


def _modinv(a, m=P):
    return pow(a % m, m - 2, m)


def _point_add(p1, p2):
    if p1 is None:
        return p2
    if p2 is None:
        return p1
    x1, y1 = p1
    x2, y2 = p2
    if x1 == x2 and y1 != y2:
        return None
    if p1 == p2:
        lam = (3 * x1 * x1 * _modinv(2 * y1)) % P
    else:
        lam = ((y2 - y1) * _modinv(x2 - x1)) % P
    x3 = (lam * lam - x1 - x2) % P
    y3 = (lam * (x1 - x3) - y1) % P
    return (x3, y3)


def _scalar_mul(k, point=(GX, GY)):
    result = None
    addend = point
    while k:
        if k & 1:
            result = _point_add(result, addend)
        addend = _point_add(addend, addend)
        k >>= 1
    return result


def _bytes_from_int(x, length=32):
    return x.to_bytes(length, "big")


def _int_from_bytes(b):
    return int.from_bytes(b, "big")


def pubkey_from_priv(priv_hex):
    priv_int = _int_from_bytes(bytes.fromhex(priv_hex))
    if priv_int == 0 or priv_int >= N:
        raise ValueError("Invalid private key")
    pub_point = _scalar_mul(priv_int)
    pub_x, pub_y = pub_point
    return bytes([0x04]) + _bytes_from_int(pub_x) + _bytes_from_int(pub_y)


# =============================================================================
# GENESIS WALLET — keep these secret and secure!
# =============================================================================
# The private key below controls the 5 Elek genesis reward output.
# You MUST back up this file or the generated results file.

GENESIS_PUBKEY_HEX = (
    "0417256d59a30a1849f1fbbbc507e1c5dabb91140de37b0860b86c778cf8403ad"
    "3a70317000e6212f41fcc50b2f05f607454d1206eac580ddfde351c5578479de3"
)

ALPHABET = (
    "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"
)


def b58encode(data):
    num = int.from_bytes(data, "big")
    result = ""
    while num > 0:
        num, rem = divmod(num, 58)
        result = ALPHABET[rem] + result
    for b in data:
        if b == 0:
            result = "1" + result
        else:
            break
    return result


def b58check(data):
    checksum = hashlib.sha256(hashlib.sha256(data).digest()).digest()[:4]
    return b58encode(data + checksum)


def compute_addresses(priv_hex):
    """Derive spendable addresses from the provided genesis keypair."""
    priv = bytes.fromhex(priv_hex)
    pub = bytes.fromhex(GENESIS_PUBKEY_HEX)

    # WIF (uncompressed)
    wif = b58check(bytes([0x80]) + priv)

    # P2PKH
    sha256_pub = hashlib.sha256(pub).digest()
    ripemd160_pub = hashlib.new("ripemd160", sha256_pub).digest()
    p2pkh = b58check(bytes([0x00]) + ripemd160_pub)

    # Bech32 (P2WPKH)
    charset = "qpzry9x8gf2tvdw0s3jn54khce6mua7l"

    def polymod(values):
        gen = [0x3b6a57b2, 0x26508e6d, 0x1ea119fa, 0x3d4233dd, 0x2a1462b3]
        chk = 1
        for v in values:
            b = chk >> 25
            chk = ((chk & 0x1ffffff) << 5) ^ v
            for i in range(5):
                chk ^= gen[i] if ((b >> i) & 1) else 0
        return chk

    def hrp_expand(hrp):
        return [ord(x) >> 5 for x in hrp] + [0] + [ord(x) & 31 for x in hrp]

    def create_checksum(hrp, data):
        values = hrp_expand(hrp) + data
        polymod_val = polymod(values + [0, 0, 0, 0, 0, 0]) ^ 1
        return [(polymod_val >> (5 * (5 - i))) & 31 for i in range(6)]

    def encode_bech32(hrp, data):
        combined = data + create_checksum(hrp, data)
        return hrp + "1" + "".join(charset[d] for d in combined)

    def convertbits(data, frombits, tobits, pad=True):
        acc = 0
        bits = 0
        ret = []
        max_acc = (1 << (frombits + tobits - 1)) - 1
        for value in data:
            acc = ((acc << frombits) | value) & max_acc
            bits += frombits
            while bits >= tobits:
                bits -= tobits
                ret.append((acc >> bits) & ((1 << tobits) - 1))
        if pad and bits:
            ret.append((acc << (tobits - bits)) & ((1 << tobits) - 1))
        return ret

    p2wpkh_data = [0] + convertbits(ripemd160_pub, 8, 5)
    p2wpkh = encode_bech32("be", p2wpkh_data)

    return wif, p2pkh, p2wpkh


# =============================================================================
# Mining helpers
# =============================================================================

def double_sha256(data):
    return hashlib.sha256(hashlib.sha256(data).digest()).digest()


def bits_to_target(n_bits):
    exponent = (n_bits >> 24) & 0xff
    coefficient = n_bits & 0x007fffff
    return coefficient * (2 ** (8 * (exponent - 3)))


def target_to_hex(target):
    h = hex(target)[2:].rstrip("L")
    if len(h) % 2:
        h = "0" + h
    return h.zfill(64)


def uint256_to_hex(data):
    return data[::-1].hex()


def make_varint(n):
    if n < 0xfd:
        return bytes([n])
    elif n <= 0xffff:
        return b"\xfd" + struct.pack("<H", n)
    elif n <= 0xffffffff:
        return b"\xfe" + struct.pack("<I", n)
    else:
        return b"\xff" + struct.pack("<Q", n)


def build_tx_mainnet(reward=500_000_000):
    """Build the standard genesis transaction used by mainnet/testnet3/signet."""
    version = struct.pack("<I", 1)

    vin_count = make_varint(1)
    prev_txid = b"\x00" * 32
    prev_vout = struct.pack("<I", 0xffffffff)

    psz = b"Mathematics secures your money. Time erases your traces. You own the moment."
    script_sig = (
        bytes([0x04, 0xff, 0xff, 0x00, 0x1d, 0x01, 0x04])
        + bytes([len(psz)])
        + psz
    )
    script_sig_len = make_varint(len(script_sig))
    sequence = struct.pack("<I", 0xffffffff)

    vout_count = make_varint(1)
    value = struct.pack("<Q", reward)

    pubkey_hex = GENESIS_PUBKEY_HEX
    script_pub = bytes([0x41]) + bytes.fromhex(pubkey_hex) + bytes([0xac])
    script_pub_len = make_varint(len(script_pub))

    locktime = struct.pack("<I", 0)

    tx = version + vin_count + prev_txid + prev_vout + script_sig_len + script_sig + sequence
    tx += vout_count + value + script_pub_len + script_pub + locktime
    return tx


def build_tx_testnet4(reward=500_000_000):
    """Build the Testnet4 genesis transaction."""
    version = struct.pack("<I", 1)

    vin_count = make_varint(1)
    prev_txid = b"\x00" * 32
    prev_vout = struct.pack("<I", 0xffffffff)

    psz = b"03/May/2024 000000000000000000001ebd58c244970b3aa9d783bb001011fbe8ea8e98e00e"
    script_sig = (
        bytes([0x04, 0xff, 0xff, 0x00, 0x1d, 0x01, 0x04])
        + bytes([len(psz)])
        + psz
    )
    script_sig_len = make_varint(len(script_sig))
    sequence = struct.pack("<I", 0xffffffff)

    vout_count = make_varint(1)
    value = struct.pack("<Q", reward)

    script_pub = bytes([0x20]) + b"\x00" * 32 + bytes([0xac])
    script_pub_len = make_varint(len(script_pub))

    locktime = struct.pack("<I", 0)

    tx = version + vin_count + prev_txid + prev_vout + script_sig_len + script_sig + sequence
    tx += vout_count + value + script_pub_len + script_pub + locktime
    return tx


def mine_genesis(name, timestamp, n_bits, tx_builder, reward=500_000_000):
    print(f"\n=== Mining {name} genesis block ===")

    tx = tx_builder(reward)
    merkle_root = double_sha256(tx)
    print(f"Merkle root: {uint256_to_hex(merkle_root)}")

    target = bits_to_target(n_bits)
    print(f"Target:      {target_to_hex(target)}")

    version = struct.pack("<I", 1)
    prev_block = b"\x00" * 32
    merkle_root_le = merkle_root
    ts = struct.pack("<I", timestamp)
    bits = struct.pack("<I", n_bits)

    nonce = 0
    start = time.time()
    while nonce < 0xffffffff:
        nonce_bytes = struct.pack("<I", nonce)
        header = version + prev_block + merkle_root_le + ts + bits + nonce_bytes
        h = double_sha256(header)

        if int.from_bytes(h, "little") <= target:
            elapsed = time.time() - start
            print(f"FOUND! nonce={nonce}  hash={uint256_to_hex(h)}  time={elapsed:.2f}s")
            return nonce, uint256_to_hex(h), uint256_to_hex(merkle_root)

        nonce += 1
        if nonce % 1_000_000 == 0:
            elapsed = time.time() - start
            rate = nonce / elapsed
            print(f"  tried {nonce} nonces... {rate:,.0f} H/s")

    print("FAILED to find nonce in 2^32 attempts")
    return None, None, None


# =============================================================================
# Results file writer
# =============================================================================

def write_results(filename, data, priv_hex, wif, p2pkh, p2wpkh):
    with open(filename, "w") as f:
        f.write(
"""# =============================================================================
# ELEKTRON NET GENESIS RESULTS — KEEP THIS FILE SECURE!
# =============================================================================
# Generated: {timestamp}
# This file contains:
#   - Genesis block parameters for all networks
#   - Wallet credentials for the genesis reward (5 Elek)
#   - Copy-paste code for src/kernel/chainparams.cpp
# =============================================================================


# =============================================================================
# WALLET CREDENTIALS — DO NOT SHARE
# =============================================================================

Genesis Private Key (hex):
  {priv_hex}

Genesis Private Key (WIF uncompressed):
  {wif}

Genesis Public Key (hex uncompressed):
  {pub_hex}

Addresses derived from this keypair:
  P2PKH:    {p2pkh}
  P2WPKH:   {p2wpkh}

Note: The actual genesis output script is P2PK (pay-to-pubkey), not P2PKH
or P2WPKH.  Standard wallets will NOT recognize this output automatically.
To spend it you must build a raw transaction where the scriptSig is exactly
"<pubkey> OP_CHECKSIG".  Use `elektron-tx` or a raw-transaction library.


# =============================================================================
# GENESIS BLOCK PARAMETERS
# =============================================================================

| Network  | nTime      | nNonce       | hashGenesisBlock (uint256 BE)              | hashMerkleRoot (uint256 BE)                  |
|----------|------------|--------------|----------------------------------------------|----------------------------------------------|
| Mainnet  | {main_time:10} | {main_nonce:12} | {main_hash} | {main_merkle} |
| Testnet3 | {test3_time:10} | {test3_nonce:12} | {test3_hash} | {test3_merkle} |
| Testnet4 | {test4_time:10} | {test4_nonce:12} | {test4_hash} | {test4_merkle} |
| Signet   | {signet_time:10} | {signet_nonce:12} | {signet_hash} | {signet_merkle} |


# =============================================================================
# COPY-PASTE CODE FOR src/kernel/chainparams.cpp
# =============================================================================

--- Mainnet (around line 122) ---
    genesis = CreateGenesisBlock({main_time}, {main_nonce}, 0x1d0a0000, 1, 5 * COIN);
    consensus.hashGenesisBlock = genesis.GetHash();
    assert(consensus.hashGenesisBlock == uint256{{"{main_hash}"}});
    assert(genesis.hashMerkleRoot == uint256{{"{main_merkle}"}});

--- Testnet3 (around line 238) ---
    genesis = CreateGenesisBlock({test3_time}, {test3_nonce}, 0x1d0a0000, 1, 5 * COIN);
    consensus.hashGenesisBlock = genesis.GetHash();
    assert(consensus.hashGenesisBlock == uint256{{"{test3_hash}"}});
    assert(genesis.hashMerkleRoot == uint256{{"{test3_merkle}"}});

--- Testnet4 (around line 340) ---
    genesis = CreateGenesisBlock(testnet4_genesis_msg,
            testnet4_genesis_script,
            {test4_time},
            {test4_nonce},
            0x1d0a0000,
            1,
            5 * COIN);
    consensus.hashGenesisBlock = genesis.GetHash();
    assert(consensus.hashGenesisBlock == uint256{{"{test4_hash}"}});
    assert(genesis.hashMerkleRoot == uint256{{"{test4_merkle}"}});

--- Signet (around line 478) ---
    genesis = CreateGenesisBlock({signet_time}, {signet_nonce}, 0x1d0a0000, 1, 5 * COIN);
    consensus.hashGenesisBlock = genesis.GetHash();
    assert(consensus.hashGenesisBlock == uint256{{"{signet_hash}"}});
    assert(genesis.hashMerkleRoot == uint256{{"{signet_merkle}"}});

--- Regtest (around line 589) ---
    genesis = CreateGenesisBlock(1296688602, 2, 0x207fffff, 1, 5 * COIN);
    // Regtest hashes stay as-is (instant PoW, no real mining)

# =============================================================================
""".format(
                timestamp=time.strftime("%Y-%m-%d %H:%M:%S UTC", time.gmtime()),
                priv_hex=priv_hex,
                wif=wif,
                pub_hex=GENESIS_PUBKEY_HEX,
                p2pkh=p2pkh,
                p2wpkh=p2wpkh,
                main_time=data["mainnet"]["time"],
                main_nonce=data["mainnet"]["nonce"],
                main_hash=data["mainnet"]["hash"],
                main_merkle=data["mainnet"]["merkle"],
                test3_time=data["testnet3"]["time"],
                test3_nonce=data["testnet3"]["nonce"],
                test3_hash=data["testnet3"]["hash"],
                test3_merkle=data["testnet3"]["merkle"],
                test4_time=data["testnet4"]["time"],
                test4_nonce=data["testnet4"]["nonce"],
                test4_hash=data["testnet4"]["hash"],
                test4_merkle=data["testnet4"]["merkle"],
                signet_time=data["signet"]["time"],
                signet_nonce=data["signet"]["nonce"],
                signet_hash=data["signet"]["hash"],
                signet_merkle=data["signet"]["merkle"],
            )
        )
    print(f"\nResults written to: {os.path.abspath(filename)}")


# =============================================================================
# Main
# =============================================================================

if __name__ == "__main__":
    n_bits = 0x1d7fffff
    current_time = int(time.time())

    # Interactive private key input
    print("=" * 70)
    print("ELEKTRON NET GENESIS MINER")
    print("=" * 70)
    print("\nPlease enter the genesis private key (64 hex characters).")
    print("This key controls the 5 Elek genesis reward.")
    priv_input = input("Private key (hex): ").strip().lower()
    if len(priv_input) != 64:
        print("Error: Private key must be exactly 64 hex characters.")
        sys.exit(1)
    try:
        bytes.fromhex(priv_input)
    except ValueError:
        print("Error: Invalid hex string.")
        sys.exit(1)

    # Verify private key matches the hardcoded public key
    derived_pub = pubkey_from_priv(priv_input)
    if derived_pub.hex() != GENESIS_PUBKEY_HEX:
        print("Error: Derived public key does not match the hardcoded genesis pubkey.")
        print("Make sure you entered the correct private key.")
        sys.exit(1)
    print("Private key verified successfully.")

    # Derive wallet addresses
    wif, p2pkh, p2wpkh = compute_addresses(priv_input)

    print(f"\nGenesis pubkey: {GENESIS_PUBKEY_HEX[:20]}...{GENESIS_PUBKEY_HEX[-20:]}")
    print(f"P2WPKH address: {p2wpkh}")
    print(f"\nMining with nBits=0x{n_bits:08x} (target ~10x easier than Bitcoin)")
    print(f"Reward = 5 Elek = 500,000,000 Lepton")

    # Each network MUST have a distinct genesis hash so peers can tell chains apart.
    # We achieve this by giving each network a different timestamp, offset by 137 s
    # (a nod to the 137-day pruning window).
    mainnet_time = current_time
    testnet3_time = current_time + 137
    testnet4_time = current_time + 274
    signet_time = current_time + 411

    # Mine mainnet (same tx as testnet3/signet, but different timestamp)
    mainnet_nonce, mainnet_hash, mainnet_merkle = mine_genesis(
        "Mainnet", mainnet_time, n_bits, build_tx_mainnet)

    # Mine testnet3 (same tx as mainnet, but different timestamp)
    testnet3_nonce, testnet3_hash, testnet3_merkle = mine_genesis(
        "Testnet3", testnet3_time, n_bits, build_tx_mainnet)

    # Mine testnet4 (different tx AND different timestamp)
    testnet4_nonce, testnet4_hash, testnet4_merkle = mine_genesis(
        "Testnet4", testnet4_time, n_bits, build_tx_testnet4)

    # Mine signet (same tx as mainnet, but different timestamp)
    signet_nonce, signet_hash, signet_merkle = mine_genesis(
        "Signet", signet_time, n_bits, build_tx_mainnet)

    results = {
        "mainnet": {
            "time": mainnet_time,
            "nonce": mainnet_nonce,
            "hash": mainnet_hash,
            "merkle": mainnet_merkle,
        },
        "testnet3": {
            "time": testnet3_time,
            "nonce": testnet3_nonce,
            "hash": testnet3_hash,
            "merkle": testnet3_merkle,
        },
        "testnet4": {
            "time": testnet4_time,
            "nonce": testnet4_nonce,
            "hash": testnet4_hash,
            "merkle": testnet4_merkle,
        },
        "signet": {
            "time": signet_time,
            "nonce": signet_nonce,
            "hash": signet_hash,
            "merkle": signet_merkle,
        },
    }

    print("\n" + "=" * 70)
    print("RESULTS SUMMARY")
    print("=" * 70)
    print(f"Mainnet:   time={current_time} nonce={mainnet_nonce} hash={mainnet_hash} merkle={mainnet_merkle}")
    print(f"Testnet3:  time={current_time} nonce={testnet3_nonce} hash={testnet3_hash} merkle={testnet3_merkle}")
    print(f"Testnet4:  time={current_time} nonce={testnet4_nonce} hash={testnet4_hash} merkle={testnet4_merkle}")
    print(f"Signet:    time={current_time} nonce={signet_nonce} hash={signet_hash} merkle={signet_merkle}")

    write_results("genesis_results.txt", results, priv_input, wif, p2pkh, p2wpkh)

    print("\n" + "=" * 70)
    print("NEXT STEPS")
    print("=" * 70)
    print("1. Open genesis_results.txt and copy the code blocks into")
    print("   src/kernel/chainparams.cpp (replace the placeholder lines).")
    print("2. Rebuild the project: cmake --build .")
    print("3. Start the node: ./src/elektrond")
    print("4. Keep genesis_results.txt SECURE — it holds your private key!")
