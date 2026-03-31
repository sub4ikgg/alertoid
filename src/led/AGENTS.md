<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-03-31 | Updated: 2026-03-31 -->

# src/led

## Purpose
Controls the three status LEDs (Yellow GPIO 27, Green GPIO 25, Red GPIO 32) and provides named blink pattern functions that encode device state. Called from `main.cpp` and `resource.cpp`.

## Key Files

| File | Description |
|------|-------------|
| `led.h` | Public API: `preparePinMode`, `testLeds`, blink functions, raw `toggleXxxPin` helpers |
| `led.cpp` | Pin assignments, all blink pattern implementations using `digitalWrite` + `delay` |

## For AI Agents

### Working In This Directory
- All blink functions are **blocking** — they use `delay()` and do not return until the pattern completes. Keep this in mind for loop timing.
- `waitingForBleBlink()` is `static` and currently unreachable — it was likely an earlier prototype. Do not call it without updating `docs/led-blink-spec.md`.
- The resource-OK blink (3× green) is implemented **inline in `resource.cpp`**, not here. If refactoring, move it to a named function in this module and update the spec.
- Pin constants (`Y_PIN`, `G_PIN`, `R_PIN`) are `const` file-scope in `led.cpp` — not exposed in the header. Reference them only through the toggle helpers.

### Testing Requirements
- `testLeds()` runs at startup and blinks all three LEDs twice. Use this to confirm GPIO wiring.
- Verify each state pattern matches `docs/led-blink-spec.md` timing tables exactly.

### Common Patterns
- All patterns begin by turning all LEDs off (explicit `toggleXxxPin(false)` calls) before applying the new pattern, except `waitingForWifiBlink()` which only forces Red off.
- Raw toggle helpers (`toggleYellowPin`, `toggleGreenPin`, `toggleRedPin`) are public and used by `main.cpp` during BOOT button detection.

## Dependencies

### Internal
- Used by `src/main.cpp` and `src/resource/resource.cpp`

### External
- `Arduino.h` — `pinMode`, `digitalWrite`, `delay`

<!-- MANUAL: Any manually added notes below this line are preserved on regeneration -->
