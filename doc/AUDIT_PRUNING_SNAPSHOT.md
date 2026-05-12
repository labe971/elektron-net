# Technical Audit Report: Elektron Net v3.0.1
## Pruning, UTXO-Checkpoints & Automatic Snapshot Synchronization

**Date:** 2026-05-12 (Audit) / 2026-05-12 (Implementation completed)  
**Auditor:** kutlusoy
**Scope:** `src/validation.{h,cpp}`, `src/net_processing.{h,cpp}`, `src/init.cpp`, `src/node/{miner,blockstorage,chainstate}.cpp`, `src/protocol.h`, `src/kernel/chainparams.cpp`, `src/test/elektron_simulation.cpp`, `src/test/peerman_tests.cpp`  
**Mandate:** Audit + implementation of all P0 fixes and selected P2 measures. Build successfully compiled.

---

## 1. Definition & Usage of `MANDATORY_PRUNE_DEPTH`

### 1.1 Definition
```cpp
// src/validation.h:79
static const unsigned int MANDATORY_PRUNE_DEPTH = 197280;
```
Corresponds to **137 days** at a block time of 60 seconds (`137 * 24 * 60 * 60 / 60 = 197280`).

### 1.2 Usage Locations (Complete)

| File | Line | Purpose |
|------|------|---------|
| `src/validation.h` | 79 | **Definition** of the constant |
| `src/validation.cpp` | 2298 | `ValidateUTXOCheckpoint`: Only at heights `nHeight % 197280 == 0` a checkpoint is expected in the coinbase |
| `src/validation.cpp` | 2367 | `WriteAutomaticSnapshot`: Snapshot is only written at checkpoint heights (except `force=true`) |
| `src/validation.cpp` | 6580 | `GetPruneRange`: Maximum allowed prune = `tip - 197280` |
| `src/node/miner.cpp` | 207 | `CreateNewBlock`: Embedding the UTXO hash into the coinbase at every checkpoint |
| `src/node/blockstorage.cpp` | 359 | `PruneBlockFiles`: Mandatory pruning of all files older than `tip - 197280`, regardless of disk space |
| `src/net_processing.cpp` | 2160 | `MaybeRequestSnapshot`: Bootstrap trigger when `header_height - tip_height > 197280` |
| `src/net_processing.cpp` | 2170 | `MaybeRequestSnapshot`: Calculation of the latest checkpoint via `(header / 197280) * 197280` |
| `src/net_processing.cpp` | 5218 | `GETUTXOSNAPSHOT` handler: Validation that requested index is actually a checkpoint |
| `src/kernel/chainparams.cpp` | 120 | `nPruneAfterHeight = 394560` (= `2 * 197280`): First pruning only after 274 days |
| `src/test/elektron_simulation.cpp` | 49, 157, 176, 184, 192, 306, 339, 352 | Unit tests for validation of the constant and its calculations |
| `src/test/blockmanager_tests.cpp` | 311, 331 | Unit tests for prune-range enforcement |

**Conclusion:** The constant is centrally and consistently used across all relevant subsystems (Consensus, Mining, P2P, Storage, Tests).

---

## 2. Architecture Overview

### 2.1 Components

| Component | Mechanism | Responsible File |
|-----------|-----------|------------------|
| **Mandatory Pruning** | All blocks older than 137 days are deleted | `src/node/blockstorage.cpp` |
| **UTXO-Checkpoint in Coinbase** | Every 197,280 blocks: OP_RETURN with `height + hash_serialized(UTXO-Set)` | `src/node/miner.cpp`, `src/validation.cpp` |
| **Automatic Snapshot Generation** | After `ConnectBlock` at checkpoint: `.dat` + `.hash` sidecar | `src/validation.cpp` |
| **Snapshot P2P Download** | `GETUTXOSNAPSHOT` → `UTXOSNAPSHOT` → `GETSNAPSHOTDATA` → `SNAPSHOTDATA` | `src/net_processing.cpp`, `src/protocol.h` |
| **Snapshot Activation** | `ActivateSnapshot()` with `verify_assumeutxo_hash=false`, background validation disabled | `src/init.cpp`, `src/validation.cpp` |

---

## 3. Scenario Analysis: Code Paths

### 3.1 Scenario A: "Nobody in the network has the full chain anymore"

**Code path:**
1. New node starts IBD. Header sync runs.
2. `MaybeRequestSnapshot()` (`src/net_processing.cpp:2137`) is called every 60 seconds by the scheduler.
3. Trigger condition: `header_height - tip_height > MANDATORY_PRUNE_DEPTH` (line 2160).
4. Target checkpoint: `(header_height / 197280) * 197280` (line 2170). This is the **latest** checkpoint that is not ahead of the best header.
5. `GETUTXOSNAPSHOT` is broadcast to **all** connected peers (line 2212).
6. Responding peers are recorded in `m_snapshot_peers[checkpoint_hash]` (line 5265).
7. Download state is initialized (`.download` temp file, line 5281).
8. In `SendMessages()` (line 6598) it is checked for each peer whether they have the snapshot. Then `GETSNAPSHOTDATA` with 1-MB chunks is sent (line 6615), 10-second rate limit per peer.
9. Incoming chunks are written to the `.download` file. Upon completion it is renamed to `.dat` (line 5351).
10. `MaybeActivateAutomaticSnapshot()` (`src/init.cpp:1432`) finds the `.dat` file and activates it via `ActivateSnapshot()` with `verify_assumeutxo_hash=false` (line 1504).
11. Background validation is immediately disabled: `SetTargetBlock(nullptr)` (line 1528).

**Snapshot replacement:** If an old snapshot already exists, `ActivateSnapshot()` (`src/validation.cpp:5798`) checks: `if (!verify_assumeutxo_hash)` → old chainstate is deleted via `DeleteChainstate()`.

### 3.2 Scenario B: "Node offline >137 days, was online at multiple checkpoints"

**Path with old snapshot:**
1. Node starts with existing snapshot chainstate (e.g. base at 197,280).
2. `LoadChainstate()` (`src/node/chainstate.cpp:196`) recognizes automatic snapshot (no entry in `chainparams.AssumeutxoForHeight()`).
3. Background validation is disabled on restart (line 206).
4. Header sync: `best_header` at e.g. 800,000.
5. `MaybeRequestSnapshot()` checks: `existing_height (197280) >= target_height (789120)`? No (line 2199).
6. Log: "Existing snapshot at 197280 is behind latest checkpoint 789120. Requesting newer snapshot from peers." (line 2203).
7. `GETUTXOSNAPSHOT` is broadcast. Download starts.
8. Upon completion: `ActivateSnapshot()` calls `DeleteChainstate(old_cs)` (line 5803) and deletes the old chainstate. `SetTargetBlock(nullptr)` is called on all chainstates (line 5808).

**Path without snapshot (only old chainstate):**
1. Node starts with tip at 500,000, network at 800,000. Gap = 300,000 > 197,280.
2. `MaybeRequestSnapshot()` recognizes: No existing snapshot. Calculates target checkpoint 789,120.
3. Same bootstrap path as Scenario A.

---

## 4. Identified Risks & Weaknesses

### 4.1 CRITICAL: No Cryptographic Snapshot Hash Check — Blind Trust

**Location:** `src/init.cpp:1504`, `src/validation.cpp:5780`

`ActivateSnapshot(afile, metadata, false, /*verify_assumeutxo_hash=*/false)` completely skips the hardcoded AssumeUtxo hash check.

**Problem:**
- The on-chain checkpoint only proves that a hash exists. It does **not** prove that the downloaded `.dat` file correctly represents this hash.
- A malicious peer can create a `.dat` file with correct metadata (`base_blockhash = Checkpoint`) but manipulated UTXO data.
- Since background validation is permanently disabled for automatic snapshots (`SetTargetBlock(nullptr)`), there is **no** later validation path that would detect the manipulation.
- The node would work with a poisoned UTXO set and accept/reject transactions differently from the rest of the network.

**Post-Implementation Status:** **Resolved** by L0.1 (`src/init.cpp`). Before activation, the downloaded snapshot hash is validated against the coinbase OP_RETURN of the checkpoint block. On mismatch, the file is deleted and activation is aborted.

**Impact:** Permanent consensus split, unrecoverable without manual intervention.

### 4.2 CRITICAL: Snapshot Activation Picks Arbitrary File

**Location:** `src/init.cpp:1447-1455`

```cpp
for (const auto& entry : fs::directory_iterator(snapshot_dir)) {
    if (fname.ends_with(".dat") && !fname.ends_with(".download")) {
        snapshot_path = entry.path();
        break;   // ARBITRARY
    }
}
```

`fs::directory_iterator` yields no defined order. If multiple `.dat` files exist (remnants of old installations or earlier checkpoints), the **first arbitrary** one is activated.

**Post-Implementation Status:** **Resolved** by L0.4 (`src/init.cpp`). `MaybeActivateAutomaticSnapshot` collects all `.dat` files, parses the height from the filename, and deterministically selects the newest checkpoint. After successful activation, all obsolete files in the `snapshots/` directory are deleted.

**Impact:** Node may load an old snapshot, then need to replace it, losing time or oscillating between states. Old `.dat` and `.failed` files are **never** deleted.

### 4.3 CRITICAL: Snapshot Download Without Global Timeout / Stall Detection

**Location:** `src/net_processing.cpp:6600-6621`

- No global timeout for a download.
- No detection that a download has "stalled".
- No logic to discard an incomplete download and restart.
- `.download` file on disk persists on restart, but `received_ranges` in RAM is lost.

**Post-Implementation Status:** **Resolved** by L0.2 (`src/net_processing.cpp`). `SnapshotDownload` now has `last_progress_time`. `MaybeRequestSnapshot` checks every 60 seconds: if a download has had no progress for >30 minutes, it is discarded, the `.download` file is deleted, and a re-broadcast is allowed.

**Deadlock scenario:**
1. Node broadcasts `GETUTXOSNAPSHOT`.
2. A peer responds with `UTXOSNAPSHOT`. Node starts download.
3. This peer goes offline or never responds with `SNAPSHOTDATA`.
4. `MaybeRequestSnapshot()` is called every 60s, but sees `m_snapshot_downloads.count(checkpoint_hash)` and returns immediately (line 2182).
5. The node thus sends **no new** `GETUTXOSNAPSHOT` broadcasts.
6. If the peer that advertised the snapshot was the only one and is now offline, `m_snapshot_peers[hash]` remains empty.
7. `peer_it->second.count(node.GetId())` is always false. The node **never** requests a chunk again.
8. **Permanent deadlock:** incomplete download is not discarded, but also not continued.

### 4.4 CRITICAL: No Snapshot Availability Guarantee in the Network

**Location:** `src/net_processing.cpp:5210`, `src/validation.cpp:2365`

Not every node has every snapshot. A node only writes snapshots for checkpoints that it **itself processed live**.

**Chain-halt scenario:**
1. Network has 10 nodes. Checkpoint at 789,120 was reached 2 days ago.
2. 7 nodes were online and have `789120-xxx.dat`.
3. 3 nodes were offline and do not have this snapshot.
4. Today the 7 nodes all go offline.
5. A new node joins the network and connects to the 3 remaining nodes.
6. It broadcasts `GETUTXOSNAPSHOT` for 789,120.
7. None of the 3 peers has the file.
8. The node waits 60 seconds, broadcasts again. Again no response.
9. **This repeats indefinitely.** The node can never synchronize.

**Post-Implementation Status:** **Partially resolved** by L0.3 (`src/protocol.h`, `src/init.cpp`, `src/net_processing.cpp`). `NODE_SNAPSHOT` service bit (bit 12) signals snapshot availability. `MaybeRequestSnapshot` first broadcasts only to peers with this bit. This improves discovery, but there is still no hard guarantee that snapshots must be redundantly retained.

**Conclusion:** There is no requirement that snapshots be retained by a minimum number of nodes. The network can put itself into a configuration where no full sync is possible anymore.

### 4.5 CRITICAL: Deep Reorgs Are Impossible

**Location:** `src/init.cpp:1520`, `src/validation.cpp:6570-6591`

Since background validation is disabled and all historical blocks are pruned, a snapshot-based node has **no blocks before the checkpoint base** and **no undo data**.

**Consequence:**
- If a reorganization occurs deeper than the snapshot base (e.g. 51% attack with chain from block 789,000), a pruned node **cannot validate** this reorg.
- It cannot re-validate the old chain onto the new fork because it lacks the historical blocks and UTXO transitions.
- The entire network (almost all nodes are pruned) cannot validate the stronger fork and remains on the weaker one.

**Post-Implementation Status:** **Accepted design trade-off.** The full history is defined as the last 137 days (MANDATORY_PRUNE_DEPTH). Reorgs deeper than the latest checkpoint are by design not supported. A Stoic would say: "The past that we can no longer touch is as it is."

This is a fundamental trade-off of the pruning design, but combined with the missing background validation, the network is locked to the integrity of the latest checkpoint. An error in the checkpoint (or a malicious checkpoint miner) could permanently split the network.

### 4.6 HIGH: Double-ComputeUTXOStats in Mining Path

**Location:** `src/node/miner.cpp:208`, `src/validation.cpp:2337`

The miner calculates the UTXO hash (`ComputeUTXOStats`) and embeds it into the coinbase. `ConnectBlock` recalculates the same hash **again** for validation.

**Risk:**
- `ComputeUTXOStats` is O(n) over the entire UTXO set.
- While `ConnectBlock` runs, `cs_main` is held. While the miner calculates the hash, it blocks all processing.
- Theoretical consistency risk: If between embedding by the miner and validation by `ConnectBlock` another thread modifies the CoinsDB (not possible because of `cs_main`, but a risk with future changes), the block would be rejected as invalid and the chain would halt at the checkpoint.

**Post-Implementation Status:** **Accepted.** Pure performance problem. `cs_main` protects against inconsistencies.

### 4.7 HIGH: Snapshot Files Are Never Cleaned Up — Disk Space Leak

**Location:** `src/validation.cpp:2455`, `src/init.cpp:1509`

- On successful download: `.download` → `.dat`. Old `.dat` files remain.
- On failed activation: `.dat` → `.failed`. Never deleted.
- On snapshot replacement: `DeleteChainstate()` deletes the LevelDB data, but **not** the source `.dat` / `.hash` files in `snapshots/`.
- `WriteAutomaticSnapshot()` runs at every checkpoint. After 10 checkpoints the node has 10 `.dat` + 10 `.hash` files. At 14 GB per snapshot = 280 GB leak.

**Post-Implementation Status:** **Resolved.** `WriteAutomaticSnapshot()` now iterates the `snapshots/` directory after successful writing and deletes all files whose name does not start with the current checkpoint prefix (`<height>-<hash>`). This includes obsolete `.dat`, `.hash`, `.download`, and `.failed` files from earlier checkpoints. The cleanup runs under `cs_main` directly after the snapshot is finalized, preventing unbounded accumulation.

**Code:** `src/validation.cpp:2481-2492`

### 4.8 HIGH: Snapshot Download Tracking Lost on Restart

**Location:** `src/net_processing.cpp:5276-5285`, `src/net_processing.cpp:5336-5358`

The `received_ranges` map exists only in RAM. On node restart:
1. `.download` file remains on disk.
2. But `SnapshotDownload` is re-initialized with empty `received_ranges`.
3. The node does not know which chunks it already has.
4. It requests all chunks again. Incoming chunks are correctly written, but it is pure bandwidth waste.

**Post-Implementation Status:** **Accepted.** Functionally correct (chunks are written idempotently), but wastes bandwidth. Not classified as critical.

### 4.9 MEDIUM: `MaybeRequestSnapshot` Spam to All Peers Without Selection

**Location:** `src/net_processing.cpp:2212`

```cpp
m_connman.ForEachNode([&](CNode* pnode) {
    if (pnode) {
        MakeAndPushMessage(*pnode, NetMsgType::GETUTXOSNAPSHOT, checkpoint_hash);
    }
});
```

There is no distinction between inbound/outbound, peers that already responded vs. not, or protocol version. Every 60 seconds the same request is sent to every connected peer (up to 125 connections). For a node that is 10 days behind, that's 18,000 messages per checkpoint. Wasteful and fingerprintable.

**Post-Implementation Status:** **Partially resolved** by L0.3 (`src/net_processing.cpp`). `GETUTXOSNAPSHOT` is now first sent only to peers with `NODE_SNAPSHOT`, with fallback to outbound peers. 60-second interval is deemed acceptable.

### 4.10 MEDIUM: `NODE_NETWORK_LIMITED` Safety Window

**Location:** `src/test/peerman_tests.cpp:19`

```cpp
static constexpr int64_t NODE_NETWORK_LIMITED_ALLOW_CONN_BLOCKS = 10000000;
```

This value is explicitly set to 10,000,000 in Elektron Net (compared to Bitcoin's ~288 blocks), because "all nodes are pruned". This is correct for normal operation, but a node that accidentally does not want to load a snapshot and tries to sync the full chain would also only accept `NODE_NETWORK_LIMITED` peers, which cannot serve historical blocks before `tip - 2880`. The intended path (snapshot bootstrap) is then forced by `MaybeRequestSnapshot`.

### 4.11 MEDIUM: Missing `.hash` Sidecar File Leads to `null` Hash in P2P Handshake

**Location:** `src/net_processing.cpp:5231-5242`

If the `.hash` file is missing or unreadable, `utxo_hash.SetNull()` is sent. The receiving node accepts the snapshot anyway. The hash is only transmitted informatively, but not used for validation.

**Post-Implementation Status:** **Resolved** by L2.2 (`src/net_processing.cpp`). `ProcessGetUTXOSnapshot` now sends **no** `UTXOSNAPSHOT` response if the `.hash` sidecar file is missing or unreadable. The requesting node receives no offer and picks another peer.

---

## 5. Solution Plan — Implementation Status

### 5.1 Priority P0 (Prevent Chain Halt / Consensus Split)

#### L0.1: Validate Snapshot Hash Against On-Chain Checkpoint
**Problem:** 4.1 (Poisoned snapshots)  
**Solution implemented:**
- Before calling `ActivateSnapshot()` in `MaybeActivateAutomaticSnapshot()` (`src/init.cpp:1432`), the content of the downloaded `.dat` file is parsed and its UTXO hash is compared with the hash embedded in the coinbase of the checkpoint block.
- The coinbase checkpoint hash is already available in the block index (since the checkpoint block precedes the sync and the node has its header).
- On mismatch, the file is deleted and activation is aborted.

**Files:** `src/init.cpp`  
**Status:** **Implemented and compiled.**

#### L0.2: Download Timeout & Retry Mechanism
**Problem:** 4.3 (Deadlock with unresponsive peer)  
**Solution implemented:**
- Added a global timeout per `SnapshotDownload` (>30 minutes without progress).
- When timeout is reached:
  1. Download entry is removed from `m_snapshot_downloads`.
  2. `.download` temp file is deleted.
  3. `MaybeRequestSnapshot()` is allowed to send a new broadcast.
- Progress tracker: `last_progress_time` in `SnapshotDownload`. Updated on every incoming chunk.

**Files:** `src/net_processing.cpp` (structure `SnapshotDownload`, `MaybeRequestSnapshot`, `SNAPSHOTDATA` handler)  
**Status:** **Implemented and compiled.**

#### L0.3: Snapshot Redundancy / Seeding Requirement
**Problem:** 4.4 (No snapshot available in network)  
**Solution implemented:**
- **Option B (Hard) partially implemented:** New service bit `NODE_SNAPSHOT` (bit 12) in `src/protocol.h`.
- Nodes that currently hold at least one snapshot signal this via `nLocalServices`.
- `MaybeRequestSnapshot()` broadcasts `GETUTXOSNAPSHOT` first only to peers with `NODE_SNAPSHOT`.
- Periodic task (every 5 minutes) updates the service bit dynamically based on available snapshots.
- **Note:** This improves discovery but does not enforce a hard minimum number of snapshot-holding nodes.

**Files:** `src/protocol.h`, `src/init.cpp`, `src/net_processing.cpp`  
**Status:** **Implemented and compiled.**

#### L0.4: Deterministic Snapshot Selection
**Problem:** 4.2 (Arbitrary `.dat` selection)  
**Solution implemented:**
- In `MaybeActivateAutomaticSnapshot()` (`src/init.cpp:1447`): Collects ALL `.dat` files in the `snapshots/` directory.
- Parses the filename: `<height>-<hash>.dat`.
- Selects the file with the **highest height** (newest checkpoint).
- After successful activation, deletes all obsolete `.dat`, `.hash`, `.download`, and `.failed` files for other checkpoints.

**Files:** `src/init.cpp`  
**Status:** **Implemented and compiled.**

---

### 5.2 Priority P1 (Stability & Resource Management)

#### L1.1: Snapshot File Cleanup
**Problem:** 4.7 (Disk space leak)  
**Status:** **Done.** `WriteAutomaticSnapshot()` now cleans up obsolete `.dat`, `.hash`, `.download`, and `.failed` files from the `snapshots/` directory after each successful checkpoint snapshot. Only the current checkpoint's files are preserved.

#### L1.2: Persistent Download Progress
**Problem:** 4.8 (Tracking lost on restart)  
**Status:** **Accepted.** Functionally correct (chunks are written idempotently), but wastes bandwidth. Not classified as critical per project decision.

#### L1.3: Rate-Limiting & Smart Peer Selection for Snapshot Requests
**Problem:** 4.9 (Spam)  
**Status:** **Partially resolved by L0.3.** `GETUTXOSNAPSHOT` is now sent first to `NODE_SNAPSHOT` peers, with fallback to outbound peers. 60-second interval is deemed acceptable per project decision. No further action required.

---

### 5.3 Priority P2 (Consensus Hardening & Resilience)

#### L2.1: Checkpoint Error Watchdog
**Problem:** 4.5 (Reorgs impossible, checkpoint error = halt)  
**Solution implemented:**
- **Option B (Watchdog) implemented:** When `ConnectBlock` fails at a checkpoint due to `bad-utxo-checkpoint` or `missing-utxo-checkpoint`, `FatalError()` is called with a clear message: "Checkpoint validation failed at height X. The network may be on an invalid fork or your node needs an upgrade."
- The node shuts down instead of silently remaining on the old tip.
- A human operator must then intervene (e.g. `-reindex` or upgrade).
- **Note:** This does not resolve the fundamental pruning trade-off (deep reorgs remain impossible by design), but it prevents silent operation on an invalid fork.

**Files:** `src/validation.cpp`  
**Status:** **Implemented and compiled.**

#### L2.2: Sidecar Hash Validation in P2P Handshake
**Problem:** 4.11 (Missing `.hash` file leads to `null` hash)  
**Solution implemented:**
- In `ProcessGetUTXOSnapshot` (`src/net_processing.cpp:5210`): If the `.hash` file is missing or unreadable, the peer no longer responds with `UTXOSNAPSHOT`.
- A peer that cannot provide the hash does not offer the snapshot. The requesting node then picks another peer.
- This prevents downloading snapshots that cannot later be validated against the on-chain checkpoint.

**Files:** `src/net_processing.cpp`  
**Status:** **Implemented and compiled.**

#### L2.3: `NODE_SNAPSHOT` Service Bit
**Problem:** 4.4 (No way to know who has snapshots)  
**Status:** **Fully implemented as part of L0.3.**

---

## 6. Summary of Recommended Order — Actual Implementation

| Phase | Items | Priority | Status |
|-------|-------|----------|--------|
| **1** | L0.4 (Deterministic selection), L0.2 (Download timeout) | P0 | Done |
| **2** | L0.1 (Hash validation), L2.2 (Sidecar validation) | P0 | Done |
| **3** | L0.3 (`NODE_SNAPSHOT` bit + selective broadcast) | P0 | Done |
| **4** | L2.1 (Checkpoint error watchdog) | P2 | Done |
| — | L1.1, L1.2, L1.3, L2.3 (Cleanup, persistent progress, rate-limiting) | P1/P2 | Accepted / Partially done |
| — | 4.5, 4.6, 4.8 (Deep reorgs, double-compute, restart tracking) | — | Accepted as trade-offs |
| — | 4.7 (Disk space leak) | P1 | Done |

**Key result:** All four P0 measures and both selected P2 measures have been implemented and the code compiles successfully. The remaining open items are either accepted design trade-offs or deemed non-critical by project decision.

---

*End of report.*
