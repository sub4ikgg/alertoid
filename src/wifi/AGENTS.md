<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-03-31 | Updated: 2026-03-31 -->

# src/wifi

## Purpose
Manages Wi-Fi connection lifecycle: loads SSID/passphrase from NVS, connects using the Arduino `WiFi` library, and provides a BOOT-button abort mechanism. Credentials can be updated at runtime via `writeWifiConf()`.

## Key Files

| File | Description |
|------|-------------|
| `wifi.h` | Public API: `connectToWifi`, `writeWifiConf`, `stopConnectingToWifi`, `isConnectedToWifi` |
| `wifi.cpp` | Blocking connect loop with BOOT-button and stop-flag abort, NVS credential persistence, lazy-load guard |

## For AI Agents

### Working In This Directory
- `connectToWifi()` is **blocking** — it loops until connected, BOOT button is pressed, or `stopConnectingToWifi()` is called. Each loop iteration blinks yellow and delays 500 ms + 500 ms = ~1 s per attempt.
- Credentials are lazy-loaded once (`_prefsLoaded` guard). After `writeWifiConf()` updates the in-memory strings, NVS is written immediately and `_prefsLoaded` is set — no restart needed for new credentials to take effect.
- If NVS has no credentials, `connectToWifi()` returns immediately (SSID empty check). The device will loop in `waitingForWifiBlink()` until credentials are received over BLE.
- `stopConnectingToWifi()` sets `_stopConnectingToWifi = true`; the connect loop checks it every iteration and calls `WiFi.disconnect()` before returning.
- `isConnectedToWifi()` is a thin wrapper around `WiFi.status() == WL_CONNECTED` — safe to call any time.

### Common Patterns
- NVS namespace `wifi_prefs`, keys: `wifi_ssid` (String), `wifi_passphrase` (String).
- Green LED is turned on (`toggleGreenPin(true)`) upon successful connection — this call is inside `connectToWifi()`, not in `main.cpp`.

## Dependencies

### Internal
- `led/led.h` — `toggleYellowPin()`, `toggleGreenPin()` during connect animation
- `../debug.h` — `LOG()` macro

### External
- `WiFi.h` (ESP32 Arduino)
- `Preferences` (ESP32 NVS)

<!-- MANUAL: Any manually added notes below this line are preserved on regeneration -->
