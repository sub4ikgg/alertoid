<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-03-31 | Updated: 2026-03-31 -->

# src/ble

## Purpose
BLE server implementation using the ESP32 Arduino BLE stack. Exposes a NUS-like GATT service with characteristics for writing Wi-Fi config, writing URL config, reading firmware/device info, reading current URL config, and issuing a reboot command. Manages advertising lifecycle and reboot-flag persistence.

## Key Files

| File | Description |
|------|-------------|
| `ble.h` | UUIDs for all 7 BLE characteristics; `extern` declarations for shared state flags (`isWifiConfigChanged`, `isUrlConfigChanged`, `isBleAdvertising`, `isBleDeviceConnected`); public function prototypes |
| `ble.cpp` | Full BLE server setup, all characteristic callbacks, `initBle()` / `startBleAdvertising()` / `stopBleAdvertising()` / `deinitBle()`, reboot-flag helpers, device-name and firmware-JSON builders |

## For AI Agents

### Working In This Directory
- `initBle()` is idempotent — guarded by `isBleInitialized`; safe to call multiple times.
- On BLE disconnect, `ServerCallbacks::onDisconnect()` always calls `ESP.restart()`. This is intentional — reconnecting without restart is not supported.
- The reboot command characteristic (`BLE_REBOOT_UUID`) writes `Preferences` key `ble/reboot=true` before calling `ESP.restart()`. `setup()` in `main.cpp` calls `checkAndClearBleRebootFlag()` to detect this and auto-start BLE advertising after restart.
- Config writes set `isWifiConfigChanged` / `isUrlConfigChanged` flags and copy data into `newWifiConfig` / `newUrlConfig` char arrays (max 255 bytes). Parsing happens in `main.cpp`, not here.
- BLE service requires 22 handles (`createService(uuid, 22)`) — do not reduce this count if adding characteristics.
- Device name format: `Alertoid-<MAC_HEX_UPPERCASE>` derived from `ESP.getEfuseMac()`.

### Common Patterns
- Callback classes (`ServerCallbacks`, `WifiConfCallbacks`, etc.) are allocated with `new` and passed to BLE objects — ESP32 BLE stack owns the lifetime.
- `getFirmwareJson()` returns `{"firmware":"1.0.0","mac":"...","serial":"..."}` — update `FIRMWARE_VERSION` define when bumping firmware.

## Dependencies

### Internal
- `resource/resource.h` — `UrlConfReadCallbacks` reads current URL config via `getResourceUrl()`, `getResourceExpectedCode()`, `getResourceCheckInterval()`
- `../debug.h` — `LOG()` macro

### External
- `BLEDevice`, `BLEServer`, `BLEUtils`, `BLE2902` (ESP32 Arduino BLE stack)
- `ArduinoJson` — serialize URL config JSON in `UrlConfReadCallbacks`
- `Preferences` — reboot flag in NVS namespace `ble`

## BLE UUIDs

| UUID suffix | Characteristic | Properties |
|-------------|----------------|------------|
| `...CA9E` | Service | — |
| `...02` | TX (notify) | NOTIFY |
| `...03` | Wi-Fi config write | WRITE |
| `...04` | URL config write | WRITE |
| `...05` | Firmware/device info | READ |
| `...06` | Reboot command | WRITE |
| `...07` | URL config read | READ |

<!-- MANUAL: Any manually added notes below this line are preserved on regeneration -->
