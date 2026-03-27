# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Alertoid is an ESP32-based alert device that periodically checks the HTTP status of a configurable URL and signals availability via LEDs. Configuration (Wi-Fi credentials and monitored URL) is set over BLE using a companion app.

## Build & Flash Commands

This is a PlatformIO project targeting `esp32dev`. Use the PlatformIO CLI (`pio`) or the VS Code PlatformIO extension.

```bash
# Build
pio run

# Flash to device
pio run --target upload

# Monitor serial output (115200 baud)
pio device monitor

# Build + flash + monitor in one step
pio run --target upload && pio device monitor
```

There are no automated tests — `test/` is empty.

## Architecture

The main loop in `src/main.cpp` drives three mutually exclusive operating modes:

1. **BLE configuration mode** — activated by holding BOOT button (GPIO 0) for >3 loop ticks, or automatically after a reboot triggered via BLE. BLE advertising auto-stops after 240 loop ticks (~24 sec). While BLE is active, resource checking is suspended.
2. **Wi-Fi connection mode** — when not connected to Wi-Fi and BLE is idle, `connectToWifi()` blocks until connected.
3. **Resource monitoring mode** — polls the configured URL via HTTPS every `check_interval * 10` loop ticks (each tick is ~100ms).

### Module responsibilities

- **`src/ble/ble.cpp`** — BLE server with NUS-like service UUID `6E400001-...`. Exposes write characteristics for Wi-Fi config (JSON `{"ssid","passphrase"}`), URL config (JSON `{"url","code","check_interval"}`), and reboot command. Also exposes read characteristics for firmware/device info and current URL config. On BLE disconnect, the device always `ESP.restart()`s. On reboot-command, sets a `Preferences` flag (`ble/reboot=true`) so `setup()` can detect and re-enter BLE mode after restart.
- **`src/resource/resource.cpp`** — HTTP(S) availability check using `WiFiClientSecure` (insecure/no-cert-verify). Config persisted in `Preferences` namespace `res_prefs`. Default URL: `https://httpbin.org/status/200`, default expected code: 200, default interval: 50 (= 5 sec).
- **`src/wifi/wifi.cpp`** — Wi-Fi connection using credentials from `Preferences` namespace `wifi_prefs`. `connectToWifi()` blocks in a loop; BOOT button press aborts it.
- **`src/led/led.cpp`** — Three LEDs: Yellow (GPIO 27), Green (GPIO 25), Red (GPIO 32). LED patterns encode device state: waiting for Wi-Fi (yellow+green blink), resource OK (green triple-blink), resource unavailable (red rapid blink + stays red), BLE advertising (yellow+red blink), BLE connected (alternating yellow/red).
- **`src/debug.h`** — `DEBUG` flag (currently 1); `LOG(x)` macro wraps `Serial.println`.

### Persistence (ESP32 Preferences / NVS)

| Namespace    | Keys                               | Used by       |
|--------------|------------------------------------|---------------|
| `wifi_prefs` | `wifi_ssid`, `wifi_passphrase`     | wifi module   |
| `res_prefs`  | `res_url`, `res_code`, `res_interval` | resource module |
| `ble`        | `reboot` (bool)                    | ble module    |

### BLE UUIDs

| UUID suffix | Characteristic          | Properties    |
|-------------|-------------------------|---------------|
| `...CA9E`   | Service                 | —             |
| `...02`     | TX (notify)             | NOTIFY        |
| `...03`     | Wi-Fi config write      | WRITE         |
| `...04`     | URL config write        | WRITE         |
| `...05`     | Firmware/device info    | READ          |
| `...06`     | Reboot command          | WRITE         |
| `...07`     | URL config read         | READ          |

### Key constants

- `check_interval` sent over BLE is in seconds; stored internally multiplied by 10 (each loop tick ≈ 100ms).
- `BLE_AUTO_STOP_INTERVAL = 240` ticks ≈ 24 seconds of advertising before auto-stop.
- `BLE_TOGGLE_PRESS_THRESHOLD = 3` — BOOT button must be detected pressed 4 times consecutively to start BLE.
- TLS is intentionally insecure (`secureClient.setInsecure()`).
- `board_build.partitions = min_spiffs.csv` — minimal SPIFFS partition to maximize app space.
- mbedTLS SSL buffer sizes reduced to 4096 bytes each to fit in ESP32 heap.
