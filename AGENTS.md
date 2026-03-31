<!-- Generated: 2026-03-31 | Updated: 2026-03-31 -->

# Alertoid

## Purpose
ESP32-based alert device that periodically checks the HTTP status of a configurable URL and signals availability via LEDs. Configuration (Wi-Fi credentials and monitored URL) is done over BLE using a companion app. Built with the Arduino framework via PlatformIO.

## Key Files

| File | Description |
|------|-------------|
| `platformio.ini` | PlatformIO build config: esp32dev board, min_spiffs partition, mbedTLS buffer size flags, ArduinoJson dependency |
| `src/main.cpp` | Application entry point; drives the three mutually exclusive operating modes (BLE, Wi-Fi connect, resource monitoring) |
| `src/debug.h` | `DEBUG` flag and `LOG(x)` macro that wraps `Serial.println`; set `DEBUG 0` to silence serial output |
| `CLAUDE.md` | Project guidance for Claude Code including architecture overview and build commands |
| `README.md` | Public-facing project documentation |
| `LICENSE` | Project license |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `src/` | All application source code (see `src/AGENTS.md`) |
| `docs/` | Project documentation files (see `docs/AGENTS.md`) |
| `include/` | PlatformIO global include directory — currently unused (see `include/AGENTS.md`) |
| `lib/` | PlatformIO local library directory — currently unused (see `lib/AGENTS.md`) |
| `test/` | PlatformIO test directory — currently empty (see `test/AGENTS.md`) |

## For AI Agents

### Working In This Directory
- This is a PlatformIO embedded C++ project. Use `pio run` to build, `pio run --target upload` to flash.
- The only external library is `bblanchon/ArduinoJson@^7.4.1`; all other dependencies come from the ESP32 Arduino framework.
- Do not add new source files at the root level — all code belongs under `src/`.
- `board_build.partitions = min_spiffs.csv` and reduced mbedTLS buffer sizes (4096 bytes) are intentional heap-saving choices — do not remove them.

### Testing Requirements
- There are no automated tests. Verify behavior by flashing to device and monitoring serial output (`pio device monitor`, 115200 baud).
- Use the LED patterns documented in `docs/led-blink-spec.md` to validate device state transitions.

### Common Patterns
- TLS is intentionally insecure (`secureClient.setInsecure()`) — certificate verification is skipped by design.
- Persistent config uses ESP32 `Preferences` (NVS) with namespaces: `wifi_prefs`, `res_prefs`, `ble`.
- Each loop tick is ~100 ms (enforced via `delay(100)` in the monitoring branch). Time-based intervals are expressed in ticks.
- The BOOT button (GPIO 0) is used for user interaction (BLE toggle, abort Wi-Fi connect).

## Dependencies

### Internal
- All modules are under `src/`; `main.cpp` composes them.

### External
- `espressif32` platform / `esp32dev` board (Arduino framework)
- `bblanchon/ArduinoJson@^7.4.1` — JSON parsing for BLE config payloads
- ESP32 built-ins: `WiFiClientSecure`, `HTTPClient`, `BLEDevice`, `Preferences`

<!-- MANUAL: Any manually added notes below this line are preserved on regeneration -->
