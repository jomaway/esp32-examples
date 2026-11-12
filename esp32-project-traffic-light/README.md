# Project: Traffic Light

A small project that combines everything from sections 1 and 2: a traffic light with a button that switches to a blinking-yellow **night mode**. Everything is non-blocking.

> **Learning path:** 2. Non-blocking GPIO usage – project · [← Previous: Button Toggle with Shift Register](../esp32-btn-toggle-shift) · [Next: FSM Traffic Light →](../esp32-fsm-traffic-light)

## Requirements

- **Normal mode:** red (5 s) → green (5 s) → yellow (2 s) → red …
- **Night mode:** yellow LED blinks every 500 ms, red and green are off.
- A button press toggles between normal and night mode.
- The button is debounced without `delay()`.

## What You Need

No new concepts – the project only combines what you already know:

- Several independent `millis()` timers in one program ([LED Blink with millis()](../esp32-led-blink-millis))
- Non-blocking debouncing with `buttonPressed()` – copied unchanged from [Button Toggle with millis()](../esp32-btn-toggle-millis)
- Functions with parameters: `setLights(red, yellow, green)` switches all three LEDs at once
- A number (`phase`) to remember where the traffic light currently is: `0` = red, `1` = green, `2` = yellow
- `bool` flags (`nightMode`) and `if` / `else if`

## Wiring

Same circuit as [FSM Traffic Light](../esp32-fsm-traffic-light) – you can keep it on the breadboard for the next section.

| Part          | ESP32 pin | Notes                                   |
| ------------- | --------- | --------------------------------------- |
| Red LED       | GPIO 19   | 220 Ω resistor to GND                   |
| Yellow LED    | GPIO 18   | 220 Ω resistor to GND                   |
| Green LED     | GPIO 5    | 220 Ω resistor to GND                   |
| Night button  | GPIO 34   | other side to GND, 10 kΩ pull-up to 3V3 |

Serial monitor: 115200 baud.

## How it works

`loop()` does two things on every run:

1. **Check the button.** A press flips `nightMode`. Leaving night mode restarts the cycle with a full red phase.
2. **Update the lights.** In night mode the yellow LED toggles every 500 ms. In normal mode, three `if (phase == … && time is over)` checks move the light to the next phase.

## Things to Think About

The program works, but look at what it takes to understand or change it:

- `phase == 1` – which colour was that again? The numbers are **magic numbers**: their meaning lives only in a comment.
- Night mode isn't a phase. It is a separate flag that is checked *before* the phases, and switching it on or off must also reset `phase`, `phaseStartTime` and the lights.
- Try exercise 1: adding the red+yellow phase means renumbering phases and touching several places at once.

The next section introduces the **finite state machine (FSM)**: `enum` gives the states names, `switch` replaces the `if` chain, and night mode simply becomes another state (`NIGHT`). Compare this solution with [FSM Traffic Light](../esp32-fsm-traffic-light).

## Exercises

1. Add the red+yellow phase that real (German) traffic lights have before green.
2. Add a pedestrian button that shortens the current green phase.
3. Print every state change over serial.
