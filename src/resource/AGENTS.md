<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-03-31 | Updated: 2026-03-31 -->

# src/resource

## Purpose
Performs HTTPS availability checks against a configurable URL and drives LED feedback based on the result. Persists URL, expected HTTP status code, and check interval in NVS. Lazily loads config on first use.

## Key Files

| File | Description |
|------|-------------|
| `resource.h` | Public API: `initResourceClient`, `checkResourceAvailability`, `writeResourceConf`, and getters for url/code/interval |
| `resource.cpp` | HTTP GET implementation, inline resource-OK LED pattern, NVS persistence under namespace `res_prefs`, lazy-load guard |

## For AI Agents

### Working In This Directory
- `initResourceClient(WiFiClientSecure&)` must be called from `setup()` before `checkResourceAvailability()`. It stores a pointer to the caller's `secureClient` — the object must outlive all calls.
- Config is lazy-loaded: `loadPrefsIfNeeded()` runs only when `_expectedCode == -1`. After the first load (or after `writeResourceConf()`), NVS is not re-read unless the device restarts.
- `check_interval` is stored internally as ticks (× 10), not seconds. BLE sends seconds; `main.cpp` multiplies by 10 before calling `writeResourceConf()`. Getters return ticks.
- The resource-OK LED pattern (3× green blink, stays on) is implemented inline in `checkResourceAvailability()` — not in `led.cpp`. This is the only LED pattern not in the led module.
- On HTTP error (`code < 0`), the function logs and returns without changing LED state (silent retry on next poll).
- `BOOT_BUTTON_PIN` is redeclared here (same GPIO 0) solely to guard against spurious checks during BLE startup.

### Testing Requirements
- Default URL `https://httpbin.org/status/200` returns 200 — useful for initial smoke test without custom infrastructure.
- Test with an unreachable URL to verify red rapid-blink pattern and retry behavior.
- Test `writeResourceConf()` via BLE then restart to confirm NVS persistence.

### Common Patterns
- NVS namespace `res_prefs`, keys: `res_url` (String), `res_code` (int), `res_interval` (int).
- Default values: URL `https://httpbin.org/status/200`, code `200`, interval `50` ticks (5 sec).

## Dependencies

### Internal
- `../led/led.h` — `resourceIsNotAvailableBlink()`, `toggleRedPin()`, `toggleGreenPin()`
- `../ble/ble.h` — reads `isBleAdvertising` flag to skip check during BLE init
- `../debug.h` — `LOG()` macro

### External
- `WiFiClientSecure`, `HTTPClient` (ESP32 Arduino)
- `Preferences` (ESP32 NVS)

<!-- MANUAL: Any manually added notes below this line are preserved on regeneration -->
