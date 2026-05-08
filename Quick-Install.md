# Quick Install (Dirty / Copy-Paste)

> **For:** Windows users who just cloned the repo and want a Release build + NSIS installer ASAP.
> **Assumes:** You are in "Developer PowerShell for VS" (comes with Visual Studio).

---

## 1. Install deps (one-liners)

```powershell
winget install --id Microsoft.VisualStudio.Community --override "--wait --quiet --add Microsoft.VisualStudio.Workload.NativeDesktop --add Microsoft.VisualStudio.Component.Git --includeRecommended"
winget install python3
winget install NSIS.NSIS
```

> Close and reopen your terminal after VS installs so `cmake`, `cl`, etc. are in PATH.

---

## 2. Clone & enter repo

```powershell
cd ~
git clone https://github.com/kutlusoy/elektron-net.git
cd elektron-net
```

---

## 3. Build Release

```powershell
cmake -B build --preset vs2026-static
cmake --build build --config Release -j 4
```

> If you get vcpkg errors, set the root first:  
> `$env:VCPKG_ROOT = "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\vcpkg"`

---

## 4. Generate NSIS installer

The project only auto-generates the `.nsi` for MinGW. Since you built with VS, do this **copy-paste block** to create the installer manually.

```powershell
# 1. Prepare a release/ folder the .nsi expects
mkdir -Force build/release
Copy-Item build/bin/Release/*.exe build/release/

# 2. Configure the NSIS script from the template
$src = Get-Content share/setup.nsi.in -Raw
$src = $src -replace '@CLIENT_NAME@', 'Elektron Net'
$src = $src -replace '@CLIENT_URL@', 'https://elektron-net.org'
$src = $src -replace '@abs_top_srcdir@', "$PWD"
$src = $src -replace '@abs_top_builddir@', "$PWD\build"
$src = $src -replace '@BITCOIN_GUI_NAME@', 'elektron-qt'
$src = $src -replace '@BITCOIN_WRAPPER_NAME@', 'elektron'
$src = $src -replace '@BITCOIN_DAEMON_NAME@', 'elektrond'
$src = $src -replace '@BITCOIN_CLI_NAME@', 'elektron-cli'
$src = $src -replace '@BITCOIN_TX_NAME@', 'elektron-tx'
$src = $src -replace '@BITCOIN_WALLET_TOOL_NAME@', 'elektron-wallet'
$src = $src -replace '@BITCOIN_TEST_NAME@', 'test_elektron'
$src = $src -replace '@EXEEXT@', '.exe'
$src = $src -replace '@CLIENT_VERSION_MAJOR@', '3'
$src = $src -replace '@CLIENT_VERSION_MINOR@', '0'
$src = $src -replace '@CLIENT_VERSION_BUILD@', '0'
$src = $src -replace '@CLIENT_VERSION_STRING@', '3.0.0'
$src = $src -replace '@COPYRIGHT_YEAR@', '2026'
$src = $src -replace '@COPYRIGHT_HOLDERS_FINAL@', 'The Elektron Net developers'
$src = $src -replace '@CLIENT_TARNAME@', 'elektron'
$src | Set-Content build/elektron-win64-setup.nsi -Encoding UTF8

# 3. Compile the installer
makensis build/elektron-win64-setup.nsi
```

The installer `elektron-win64-setup.exe` will appear in `build\`.

---

## 5. Install / run

```powershell
# Silent install
build\elektron-win64-setup.exe /S

# Or just run the GUI directly without installing
build\bin\Release\elektron-qt.exe
```

---

## Troubleshooting

| Problem | Fix |
|---------|-----|
| `cmake` not found | Open "Developer PowerShell for VS 2026" instead of normal PowerShell. |
| `makensis` not found | Close & reopen terminal after `winget install NSIS.NSIS`, or add `C:\Program Files (x86)\NSIS` to PATH. |
| Missing DLLs at runtime | Build with `--preset vs2026-static` (not `vs2026`). |
| vcpkg missing packages | Run `cmake -B build --preset vs2026-static` again; vcpkg installs deps automatically on first configure. |

---

*That's it. Dirty, fast, done.*
