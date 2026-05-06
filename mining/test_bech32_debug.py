#!/usr/bin/env python3
"""Debug Bech32 decode for the failing address."""

addr = 'be1qk9m8xxqfd62p7h39t58f78k0yps7mtqrnc6ram'
BECH32_CHARSET = "qpzry9x8gf2tvdw0s3jn54khce6mua7l"

def _bech32_polymod(values):
    GEN = [0x3b6a57b2, 0x26508e6d, 0x1ea119fa, 0x3d4233dd, 0x2a1462b3]
    chk = 1
    for v in values:
        b = chk >> 25
        chk = ((chk & 0x1ffffff) << 5) ^ v
        for i in range(5):
            chk ^= GEN[i] if ((b >> i) & 1) else 0
    return chk

def _bech32_hrp_expand(hrp):
    return [ord(x) >> 5 for x in hrp] + [0] + [ord(x) & 31 for x in hrp]

def _bech32_verify_checksum(hrp, data):
    polymod = _bech32_polymod(_bech32_hrp_expand(hrp) + data)
    print(f"  polymod={polymod}")
    if polymod == 1:
        return 0
    if polymod == 0x2bc830a3:
        return 1
    return -1

def _bech32_convert_bits(data, from_bits, to_bits, pad=True):
    acc = 0
    bits = 0
    ret = []
    maxv = (1 << to_bits) - 1
    max_acc = (1 << (from_bits + to_bits - 1)) - 1
    for value in data:
        if value < 0 or (value >> from_bits):
            return None
        acc = ((acc << from_bits) | value) & max_acc
        bits += from_bits
        while bits >= to_bits:
            bits -= to_bits
            ret.append((acc >> bits) & maxv)
    if pad:
        if bits:
            ret.append((acc << (to_bits - bits)) & maxv)
    return ret

def _bech32_decode(bech):
    if any(ord(x) < 33 or ord(x) > 126 for x in bech):
        return None, None
    if bech.lower() != bech and bech.upper() != bech:
        return None, None
    bech = bech.lower()
    pos = bech.rfind('1')
    if pos < 1 or pos + 7 > len(bech):
        return None, None
    hrp = bech[:pos]
    data = [BECH32_CHARSET.find(x) for x in bech[pos + 1:]]
    if any(x == -1 for x in data):
        return None, None
    print(f"  HRP={hrp}, data_len={len(data)}")
    checksum_type = _bech32_verify_checksum(hrp, data)
    print(f"  checksum_type={checksum_type}")
    if checksum_type < 0:
        return None, None
    decoded = _bech32_convert_bits(data[:-6], 5, 8, True)
    return hrp, decoded

print(f"Testing address: {addr}")
hrp, data = _bech32_decode(addr)
print(f"Result: hrp={hrp}, data={data}")
if data:
    print(f"  witness_version={data[0]}, program_len={len(data)-1}")
    print(f"  program_hex={bytes(data[1:]).hex()}")
