# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Elektron Net is a minimal fork of Bitcoin Core (C++20, CMake) with two deliberate protocol changes:
- **60-second block time** (vs. Bitcoin's 10 minutes), emission scaled proportionally.
- **Mandatory 137-day pruning** (~197,280 blocks) — transaction history is erased after this window.

The genesis block is **not yet finalized**. `src/kernel/chainparams.cpp` contains placeholder `assert(...)` lines that must be uncommented and populated with real values after running `mining/mine_genesis.py`. Until then the node cannot start.

## Build Commands

**Windows — Visual Studio (recommended)**
Open "Developer PowerShell for VS" and run:
```powershell
cmake -B build --preset vs2026-static
cmake --build build --config Release -j 4
ctest --test-dir build --build-config Release -j 4
```

If you do not want the GUI (`elektron-qt.exe`), use:
```powershell
cmake -B build --preset vs2026 -DBUILD_GUI=OFF
```

If vcpkg fails with "path too long":
```powershell
cmake -B build --preset vs2026-static -DVCPKG_INSTALL_OPTIONS="--x-buildtrees-root=C:\vcpkg"
```

**Linux / macOS**
```bash
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)
ctest
```

**Other presets**
- `dev-mode` — enables all features, tests, benchmarks, fuzzing.
- `libfuzzer` / `libfuzzer-nosan` — fuzzing builds with Clang.

## Test Commands

**Unit tests**
```bash
ctest --test-dir build --build-config Release -j 4
```

Run a single unit-test suite (Boost framework):
```bash
build/bin/test_bitcoin --run_test=getarg_tests
build/bin/test_bitcoin --run_test=getarg_tests/doubledash
```

**Functional (Python) tests**
```bash
build/test/functional/test_runner.py
build/test/functional/test_runner.py feature_rbf.py
```

On Windows, set `PYTHONUTF8=1` before running functional tests.

**Benchmarks**
```bash
build/bin/bench_elektron
```

## High-Level Architecture

The layout follows upstream Bitcoin Core:
- `src/kernel/` — consensus parameters, chainparams, block creation.
- `src/consensus/` — consensus rules, amounts, merkle.
- `src/script/` — script interpreter, standard templates.
- `src/wallet/` — wallet logic, descriptors, SQLite storage.
- `src/rpc/` — JSON-RPC server implementations.
- `src/net/` / `src/net_processing/` — P2P networking.
- `src/validation.cpp` / `validation.h` — main blockchain validation.
- `src/init.cpp` — node initialization.
- `src/qt/` — Qt6 GUI (`elektron-qt`).
- `src/test/` — C++ unit tests (Boost).
- `src/bench/` — benchmarks.
- `src/secp256k1/` — embedded secp256k1 library.
- `test/functional/` — Python integration tests.
- `mining/` — Standalone mining tools (Python genesis miner, address generator, C++ standalone miner).

## Network Parameters

| Parameter | Value |
|-----------|-------|
| P2P port | 8333 |
| RPC port | 8332 |
| Bech32 HRP | `be` |
| Max supply | 21,000,000 ELEK |
| Genesis reward | 5 ELEK |
| Halving interval | 2,102,400 blocks (~4 years) |
| Difficulty retarget | 2,016 blocks |
| DNS seed | `seed.elektron-net.org` |

## Coding Style

- C++ formatting is governed by `src/.clang-format`.
- Braces on new lines for classes, functions, methods; same line for everything else.
- 4-space indentation (no tabs), except namespaces are not indented.
- Variable / namespace names: `snake_case`. Class members: `m_` prefix. Globals: `g_` prefix.
- Constants: `ALL_CAPS`. Class / function / method names: `UpperCamelCase` (PascalCase).
- Prefer `++i`, `nullptr`, `static_assert`, named/functional casts, and list-initialization `{}`.
- See `doc/developer-notes.md` for full style rules and the `clang-format-diff` helper in `contrib/devtools/`.

## Key Files for Protocol Changes

- `src/kernel/chainparams.cpp` — Network parameters, genesis block definition, DNS seeds, port numbers.
- `src/consensus/amount.h` / `consensus.h` — Block reward, halving interval, proof-of-work limits, pruning depth.
- `src/validation.cpp` — Block validation, difficulty adjustment, pruning logic.

## Important Development Notes

- Do not commit `mining/genesis_results.txt` (contains the genesis private key).
- There is no `.github/` directory; CI must be configured manually if needed.
- Build dependencies are identical to Bitcoin Core; see `doc/build-unix.md`, `doc/build-osx.md`, `doc/build-windows.md`, and `doc/build-windows-msvc.md` for platform-specific prerequisites.
