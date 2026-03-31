<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-03-31 | Updated: 2026-03-31 -->

# src

## Purpose
Contains all application source code for the Alertoid firmware. The main loop in `main.cpp` drives three mutually exclusive operating modes — BLE configuration, Wi-Fi connection, and resource monitoring — by composing the four feature modules (ble, led, resource, wifi).

## Key Files

| File | Description |
|------|-------------|
| `main.cpp` | Entry point: `setup()` initialises peripherals and checks for BLE-reboot flag; `loop()` implements the state machine (BOOT button → BLE, Wi-Fi connect, resource poll) |
| `debug.h` | `DEBUG` compile-time flag and `LOG(x)` macro; included by all modules for serial logging |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `ble/` | BLE server, characteristics, callbacks, and reboot-flag logic (see `ble/AGENTS.md`) |
| `led/` | LED GPIO control and blink patterns for every device state (see `led/AGENTS.md`) |
| `resource/` | HTTPS resource availability check, config persistence, and interval management (see `resource/AGENTS.md`) |
| `wifi/` | Wi-Fi connection using stored credentials, BOOT-button abort (see `wifi/AGENTS.md`) |

## For AI Agents

### Working In This Directory
- All timing is tick-based: one loop iteration ≈ 100 ms (`delay(100)` in the monitoring branch; other branches use longer delays).
- Counters (`bleToggleCounter`, `bleAutoStopCounter`, `resourceCheckingCounter`) accumulate ticks; compare against threshold constants defined at the top of `main.cpp`.
- BLE and Wi-Fi modes are mutually exclusive — the loop returns early while BLE is active.
- Config changes received over BLE (`isWifiConfigChanged`, `isUrlConfigChanged`) are applied at the top of `loop()` on the next tick, not inside the BLE callbacks.
- `WiFiClientSecure secureClient` is created in `main.cpp` and passed into `initResourceClient()` — it is the single shared TLS client.

### Testing Requirements
- No automated tests. Flash and verify via serial monitor + LED patterns.
- Key scenarios to exercise: BOOT button hold → BLE advertise; BLE auto-stop after 240 ticks; Wi-Fi credential write → reconnect; URL config write → interval change; resource OK/fail LED patterns.

### Common Patterns
- Early-return pattern in `loop()`: each mode (BOOT press, BLE active, Wi-Fi disconnected) returns immediately, preventing overlap.
- Globals shared across modules are declared `extern` in headers (e.g., `isWifiConfigChanged`, `isBleAdvertising`).

## Dependencies

### Internal
- `ble/ble.h`, `led/led.h`, `resource/resource.h`, `wifi/wifi.h`

### External
- `WiFiClientSecure` (ESP32 Arduino)
- `ArduinoJson` (JSON parsing for BLE config payloads)

<!-- MANUAL: Any manually added notes below this line are preserved on regeneration -->
