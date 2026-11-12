# Project: Traffic Light

A small project that combines everything from sections 1 and 2: a traffic light with a button that switches to a blinking-yellow **night mode**. Everything is non-blocking.

> **Learning path:** 2. Non-blocking GPIO usage – project · [← Previous: Button Toggle with Shift Register](../esp32-btn-toggle-shift) · [Next: FSM Traffic Light →](../esp32-fsm-traffic-light)

## Requirements

- **Normal mode:** red (5 s) → green (5 s) → yellow (2 s) → red …
- **Night mode:** yellow LED blinks every 500 ms, red and green are off.
- A button press toggles between normal and night mode.
- The button is debounced without `delay()`.

## New Concepts

- Combining several independent `millis()` timers in one program
- `enum` to give states readable names (`RED`, `GREEN`, `YELLOW`)
- `switch` / `case`
- Splitting code into functions (`update_lights()`, `night_mode()`, `normal_operation()`)
- `static` local variables that keep their value between function calls
- A different board: **Seeed XIAO ESP32-S3** (Dx labels ≠ GPIO numbers)

## Wiring

Board: Seeed XIAO ESP32-S3 (`env:esp32-wokwi`)

| Part          | Board pin | GPIO | Notes                         |
| ------------- | --------- | ---- | ----------------------------- |
| Push button   | D0        | 1    | other side to GND, 1 kΩ pull-up to 3V3 |
| Red LED       | D1        | 2    | 1 kΩ resistor to GND          |
| Yellow LED    | D2        | 3    | 1 kΩ resistor to GND          |
| Green LED     | D3        | 4    | 1 kΩ resistor to GND          |

Serial monitor: 115200 baud.

## Things to Think About

The logic for "which light comes next" is spread over several functions and flags (`nightModeActive`, `traffic_light_state`). It works, but it gets hard to extend. The next section introduces the **finite state machine (FSM)** pattern as a cleaner structure – compare this solution with [FSM Traffic Light](../esp32-fsm-traffic-light).

## Exercises

1. Add the red+yellow phase that real (German) traffic lights have before green.
2. Add a pedestrian button that shortens the current green phase.
3. Print every state change over serial.
