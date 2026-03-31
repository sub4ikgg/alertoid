<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-03-31 | Updated: 2026-03-31 -->

# docs

## Purpose
Project documentation files. Currently holds the LED blink pattern specification — the authoritative reference for every LED state the device can display.

## Key Files

| File | Description |
|------|-------------|
| `led-blink-spec.md` | Detailed specification of all LED blink patterns (GPIO pins, timing, per-state behavior). Written in Russian. |

## For AI Agents

### Working In This Directory
- `led-blink-spec.md` is the source of truth for LED behavior. When modifying `src/led/led.cpp` or `src/resource/resource.cpp` (which contains an inline resource-OK blink), update this spec to match.
- Note: `waitingForBleBlink()` is documented here as unused (`static`, never called) — do not call it without also updating this spec.

### Common Patterns
- Timing tables use milliseconds; LED state transitions are listed in order of execution.

<!-- MANUAL: Any manually added notes below this line are preserved on regeneration -->
