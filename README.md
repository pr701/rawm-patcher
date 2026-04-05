# RAWM Buffer Patcher

Fixes a ~2 GB memory leak in **RAWMHUB.exe** - a native Windows mouse driver/configurator app (~50 MB) by a Chinese manufacturer. The app is required to run in the system tray for auto-profile switching, but immediately consumes **1.8–2 GB of RAM** on startup.

> [!WARNING]
> Tested on RAWMHUB version **2.0.0.3**. Newer versions should be supported since the patcher uses pattern matching rather than hardcoded offsets, but this is not guaranteed. Always check that the patcher reports **7/7 patch points found** before applying.

## Root cause

On launch, the app pre-allocates a pool of **~225 `UsbClient` objects** - one for every device model in its database, not just the ones actually connected. Each `UsbClient` constructor allocates **three 3 MB buffers** via `operator new(0x300000)`:

| Buffer | Offset | Purpose |
|--------|--------|---------|
| READ   | +848   | Incoming data from device (control channel) |
| WRITE  | +944   | Outgoing commands to device |
| LOG    | +1040  | Intermediate / staging buffer |

These buffers serve a **custom control protocol** (settings, DPI, profiles, battery status, macros, RGB) - not raw HID mouse movement data, which is handled directly by the OS via interrupt endpoints.

```
225 objects × 3 buffers × 3 MB = ~2 GB
```

The buffers are almost entirely empty (filled with zeros). Real throughput on the control channel is **~13 KB/s peak**, making 3 MB per buffer roughly **200–1000× oversized**.

## What the patcher does

Replaces the buffer size constant at **7 code locations** found via assembly pattern matching:

- **3× allocation**: `mov ecx, 0x300000` before `call operator new`
- **3× boundary check**: `cmp eax, 0x300000` in append functions (prevents heap overflow)
- **1× memset size**: `mov r8d, 0x300000` in buffer reset

Default patched value: **0x10000 (64 KB)** - provides a 5× safety margin over worst-case throughput.

```
Before: 225 × 3 × 3 145 728 B ≈ 2 023 MB
After:  225 × 3 ×    65 536 B ≈    43 MB  (-97.8%)
```

## Usage

1. Close RAWMHUB if running
2. Run `RAWMPatcher.exe`
3. The patcher auto-detects the executable; if not, click **[...]** to browse
4. Adjust the buffer size if needed (hex, default `10000` = 64 KB)
5. Click **Patch**

A `.bak` backup is created automatically before the first patch. Click **Restore** to revert to the value that was in the binary when it was loaded.

The UI language is detected automatically from Windows regional settings. To override, pass `--lang <LCID>`:

```
RAWMPatcher.exe --lang 0x0409   English
RAWMPatcher.exe --lang 0x0419   Russian
RAWMPatcher.exe --lang 0x0422   Ukrainian
RAWMPatcher.exe --lang 0x0804   Chinese Simplified
RAWMPatcher.exe --lang 0x0404   Chinese Traditional
RAWMPatcher.exe --lang 0x0407   German
RAWMPatcher.exe --lang 0x0416   Portuguese (Brazil)
```

## Building

Open `src/RAWMPatcher.sln` in Visual Studio 2022 and build for **x64** (Release or Debug). No external dependencies.

## How pattern matching works

The patcher does not rely on hardcoded file offsets. Instead, it scans the binary for four unique assembly patterns with wildcards, making it potentially compatible across different builds:

| Pattern | Signature | Matches |
|---------|-----------|---------|
| Alloc×3 | `B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 89 83 50 03 00 00 B9 ...` | 1 (3 values) |
| Check buf1 & buf3 | `42 8D 04 37 3D ?? ?? ?? ?? 7F ?? 48 8B CE` | 2 |
| Check buf2 | `42 8D 04 3F 3D ?? ?? ?? ?? 0F 8F ?? ?? ?? ?? 41 80 3E 06` | 1 |
| Memset buf3 | `49 8B C9 41 B8 ?? ?? ?? ?? 33 D2 E8 ?? ?? ?? ?? 48 8B CE` | 1 |

All 7 patch points must be found with identical values; otherwise the patcher reports an error.

## Why 64 KB is safe

The three buffers handle a **low-bandwidth control protocol**, not mouse tracking data:

- Messages are small (5–68 bytes each): DPI settings, polling rate, battery, profiles, macros, RGB
- Peak throughput: ~200 messages/sec × 68 bytes ≈ **13.6 KB/sec**
- A 1-second timer drains the buffers, so at most ~14 KB accumulates
- 64 KB provides **~5× headroom** even under worst-case load
- 8 KHz polling rate is irrelevant - it applies to the HID interrupt endpoint, not this control channel

## Notes

- This is a **workaround**, not a proper fix. The root cause is architectural: the app creates `UsbClient` objects for every supported device (not just connected ones), and each object has oversized buffers. An ideal fix would use lazy allocation or filter by connected devices.
- The patch has been tested stable with 8 KHz polling, BLE, and USB modes.
