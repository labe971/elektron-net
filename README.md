Elektron Net
============

https://elektron-net.org

What is Elektron Net?
---------------------

Elektron Net is a next-generation peer-to-peer electronic cash system.
It originated as a fork of Bitcoin Core and has since evolved into a fully
independent protocol with its own consensus rules, privacy model, and
community. It replaces proof-of-work block production with **VRF + pBFT
consensus**, adds mandatory **GDPR-by-design privacy** through stealth
addresses and 137-day pruning, and introduces a fair **epoch-based reward
system**.

> **Note:** Elektron Net is not Bitcoin Core and shares no ongoing relationship
> with the Bitcoin Core project. Issues, pull requests, and community
> discussion belong exclusively on the Elektron Net repository:
> https://github.com/kutlusoy/bitcoin-elek

Key properties:
- **No block-level PoW** — VRF leader election + pBFT finality (137-node committee)
- **60-second block time**, halving every 4 years (2,102,400 blocks)
- **21,000,000 Elek** maximum supply (1 Elek = 100,000,000 Lep)
- **5 Elek** genesis block reward (25% producer / 75% epoch pool)
- **Stealth addresses** — one-time addresses per payment (bech32m HRP: `elek1st`)
- **137-day pruning** — cryptographic forgetting, GDPR by design
- **Heartbeat system** — BLAKE3-HMAC node liveness proofs, 60-second slots
- **P2P port 9336**, network magic `ELEK` (0x454C454B)

### Genesis Block

```
Hash:        0e0033ee45b67f54c617399f01c23b3afe14e6b0de6d63ef3daf91bb15edb6d6
MerkleRoot:  37dac1cc7688adda10829bf4e35ff7ca8433124554b93837cc8a89267a40f6f1
Time:        1744156800  (2026-04-09 00:00:00 UTC)
Bits:        0x207fffff  (no PoW required)
Nonce:       0
Message:     "Elektron Net 09/Apr/2026 From the Lydian Elektron to the Digital Elek
              - Value Through Mathematics"
```

License
-------

Elektron Net is released under the terms of the MIT license. See [COPYING](COPYING)
for more information or see https://opensource.org/license/MIT.

The underlying Bitcoin Core codebase is copyright (c) 2009-present The Bitcoin Core
developers. Elektron Net extensions are copyright (c) 2026-present The Elektron Net
developers.

Building
--------

See [doc/build-unix.md](doc/build-unix.md), [doc/build-osx.md](doc/build-osx.md),
or [doc/build-windows.md](doc/build-windows.md) for build instructions.

Additional build dependencies beyond Bitcoin Core:
- **libsodium** ≥ 1.0.18 — VRF (IETF ECVRF) and Ed25519 signatures
- **BLAKE3** — heartbeat HMAC, VRF alpha computation, epoch beacon
- **blst** *(optional)* — BLS aggregate signatures for pBFT committee votes

Development Process
-------------------

The `master` branch is regularly built and tested. Tags indicate stable release
versions of Elektron Net.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).
Developer notes are in [doc/developer-notes.md](doc/developer-notes.md).

Source repository: https://github.com/kutlusoy/bitcoin-elek

Testing
-------

Unit tests:
```sh
ctest
```

Functional (Python) tests:
```sh
build/test/functional/test_runner.py
```

See [src/test/README.md](src/test/README.md) and [test/README.md](test/README.md)
for details.

Peer Discovery
--------------

On first start, Elektron Net queries DNS seeds:
- `dnsseed.elektron-net.org`
- `seed1.elektron-net.org` … `seed4.elektron-net.org`

A bootstrap endpoint is also available at:
`https://bootstrap.elektron-net.org/bootstrap.json`

See [contrib/seeds/bootstrap.php](contrib/seeds/bootstrap.php) to self-host a
seed server.

Documentation
-------------

Full protocol documentation is in [WHITEPAPER.md](WHITEPAPER.md).
Implementation workflow reference is in [WORKFLOW.md](WORKFLOW.md).
Additional technical docs are in [doc/](doc/).
