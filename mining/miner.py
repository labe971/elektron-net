#!/usr/bin/env python3
"""
Standalone CPU miner for Elektron Net.

Connects to a local node via RPC, fetches block templates, mines with
multiple threads, and submits solved blocks.
"""

import argparse
import base64
import hashlib
import json
import struct
import sys
import threading
import time
import urllib.request


def double_sha256(data: bytes) -> bytes:
    return hashlib.sha256(hashlib.sha256(data).digest()).digest()


def uint256_to_hex(data: bytes) -> str:
    return data[::-1].hex()


def build_coinbase(tx: dict) -> bytes:
    """Serialize a coinbase transaction from a block template."""
    version = struct.pack('<I', tx['version'])

    vin_count = len(tx['vin'])
    if vin_count < 0xfd:
        vin_count_bytes = bytes([vin_count])
    else:
        vin_count_bytes = b'\xfd' + struct.pack('<H', vin_count)

    inputs = b''
    for vin in tx['vin']:
        txid = bytes.fromhex(vin['txid'])[::-1]
        vout = struct.pack('<I', vin['vout'])
        script_sig = bytes.fromhex(vin['scriptSig']['hex'])
        if len(script_sig) < 0xfd:
            ss_len = bytes([len(script_sig)])
        else:
            ss_len = b'\xfd' + struct.pack('<H', len(script_sig))
        sequence = struct.pack('<I', vin['sequence'])
        inputs += txid + vout + ss_len + script_sig + sequence

    vout_count = len(tx['vout'])
    if vout_count < 0xfd:
        vout_count_bytes = bytes([vout_count])
    else:
        vout_count_bytes = b'\xfd' + struct.pack('<H', vout_count)

    outputs = b''
    for vout in tx['vout']:
        value = struct.pack('<Q', int(round(vout['value'] * 1e8)))
        script_pub = bytes.fromhex(vout['scriptPubKey']['hex'])
        if len(script_pub) < 0xfd:
            sp_len = bytes([len(script_pub)])
        else:
            sp_len = b'\xfd' + struct.pack('<H', len(script_pub))
        outputs += value + sp_len + script_pub

    locktime = struct.pack('<I', tx.get('locktime', 0))
    return version + vin_count_bytes + inputs + vout_count_bytes + outputs + locktime


def build_block_header(template: dict, nonce: int, timestamp: int = None) -> bytes:
    version = struct.pack('<i', template['version'])
    prev_block = bytes.fromhex(template['previousblockhash'])[::-1]
    merkle_root = bytes.fromhex(template['merkleroot'])[::-1]
    ts = struct.pack('<I', timestamp if timestamp is not None else template['curtime'])
    bits = struct.pack('<I', int(template['bits'], 16))
    nonce_bytes = struct.pack('<I', nonce)
    return version + prev_block + merkle_root + ts + bits + nonce_bytes


def bits_to_target(n_bits: int) -> int:
    exponent = (n_bits >> 24) & 0xff
    coefficient = n_bits & 0x007fffff
    return coefficient * (2 ** (8 * (exponent - 3)))


class RpcClient:
    def __init__(self, url: str, user: str, password: str):
        self.url = url
        credentials = base64.b64encode(f"{user}:{password}".encode()).decode()
        self.headers = {
            'Content-Type': 'application/json',
            'Authorization': f'Basic {credentials}',
        }

    def call(self, method: str, *params):
        payload = json.dumps({
            'jsonrpc': '2.0',
            'id': 1,
            'method': method,
            'params': list(params),
        }).encode()
        req = urllib.request.Request(self.url, data=payload, headers=self.headers)
        with urllib.request.urlopen(req, timeout=30) as resp:
            result = json.loads(resp.read().decode())
            if 'error' in result and result['error'] is not None:
                raise RuntimeError(result['error'])
            return result['result']


def mine_worker(tid: int, header: bytes, target: int, start_nonce: int, stop_event: threading.Event, result: list):
    nonce = start_nonce
    while not stop_event.is_set() and nonce < 0xffffffff:
        nonce_bytes = struct.pack('<I', nonce)
        h = double_sha256(header[:76] + nonce_bytes)
        if int.from_bytes(h, 'little') <= target:
            result.append((nonce, uint256_to_hex(h)))
            stop_event.set()
            return
        nonce += 1
        if nonce % 1_000_000 == 0 and tid == 0:
            print(f"  [thread {tid}] tried {nonce:,} nonces...")


def mine_block(template: dict, num_threads: int) -> tuple:
    target = bits_to_target(int(template['bits'], 16))
    print(f"Target: {target:064x}")

    stop_event = threading.Event()
    result = []
    threads = []

    for i in range(num_threads):
        header = build_block_header(template, 0)
        start_nonce = i
        t = threading.Thread(target=mine_worker, args=(i, header, target, start_nonce, stop_event, result))
        threads.append(t)
        t.start()

    for t in threads:
        t.join()

    if result:
        nonce, block_hash = result[0]
        print(f"FOUND! nonce={nonce} hash={block_hash}")
        return nonce
    return None


def submit_block(rpc: RpcClient, template: dict, nonce: int):
    template['nonce'] = nonce
    block_hex = assemble_block(template)
    try:
        rpc.call('submitblock', block_hex)
        print("Block submitted successfully.")
    except Exception as e:
        print(f"Submit failed: {e}")


def assemble_block(template: dict) -> str:
    """Re-assemble block from template + solved nonce into hex."""
    coinbase_tx = build_coinbase(template['transactions'][0] if template.get('transactions') else template['coinbasetxn'])
    transactions = [coinbase_tx] + [bytes.fromhex(tx['data']) for tx in template.get('transactions', [])[1:]]

    version = struct.pack('<i', template['version'])
    prev_block = bytes.fromhex(template['previousblockhash'])[::-1]
    merkle_root = bytes.fromhex(template['merkleroot'])[::-1]
    ts = struct.pack('<I', template['curtime'])
    bits = struct.pack('<I', int(template['bits'], 16))
    nonce_bytes = struct.pack('<I', template.get('nonce', 0))
    header = version + prev_block + merkle_root + ts + bits + nonce_bytes

    tx_count = len(transactions)
    if tx_count < 0xfd:
        tx_count_bytes = bytes([tx_count])
    else:
        tx_count_bytes = b'\xfd' + struct.pack('<H', tx_count)

    txs = b''.join(transactions)
    block = header + tx_count_bytes + txs
    return block.hex()


def main():
    parser = argparse.ArgumentParser(description="Standalone Elektron Net CPU miner")
    parser.add_argument('--url', default='http://127.0.0.1:8332', help='RPC URL')
    parser.add_argument('--user', default='user', help='RPC username')
    parser.add_argument('--password', default='password', help='RPC password')
    parser.add_argument('--threads', type=int, default=4, help='Number of mining threads')
    parser.add_argument('--continuous', action='store_true', help='Mine continuously')
    args = parser.parse_args()

    rpc = RpcClient(args.url, args.user, args.password)

    print(f"Elektron Net Miner (Python)")
    print(f"Network:   Elektron Net (60s blocks, 5 Elek reward)")
    print(f"RPC:       {args.url}")
    print(f"Threads:   {args.threads}")

    while True:
        try:
            print("\nFetching block template...")
            template = rpc.call('getblocktemplate', {'rules': ['segwit']})
        except Exception as e:
            print(f"Failed to get block template: {e}")
            time.sleep(5)
            continue

        nonce = mine_block(template, args.threads)
        if nonce is not None:
            submit_block(rpc, template, nonce)
        else:
            print("No nonce found (template expired?).")

        if not args.continuous:
            break
        time.sleep(1)


if __name__ == '__main__':
    main()
