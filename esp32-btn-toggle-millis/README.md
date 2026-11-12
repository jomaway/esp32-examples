# ESP32 Button Toggle with `millis()`

Toggle an LED with a button – this time with **non-blocking debouncing** based on `millis()` instead of `delay()`.

> **Learning path:** 2. Non-blocking GPIO usage · [← Previous: LED Blink with millis()](../esp32-led-blink-millis) · [Next: Button Toggle with Shift Register →](../esp32-btn-toggle-shift)

## New Concepts

- Debouncing without blocking: a signal is only accepted once it has been **stable** for a certain time
- Separating the *raw* state (`lastButtonState`) from the *debounced* state (`lastStableButtonState`)

## Wiring

| Part        | ESP32 pin | Notes                             |
| ----------- | --------- | --------------------------------- |
| LED         | GPIO 18   | through a 220 Ω resistor to GND   |
| Push button | GPIO 34   | other side to GND                 |
| Pull-up     | GPIO 34   | 10 kΩ resistor to 3V3             |

## How it works

1. Every change of the raw button reading (also bounces) restarts the timer `lastChangeTime`.
2. Only when the reading hasn't changed for 50 ms is it treated as *stable*.
3. A falling edge of the **stable** state (`HIGH → LOW`) toggles the LED.

The `loop()` never waits, so other tasks keep running smoothly.

## Exercises

1. Combine this example with [LED Blink with millis()](../esp32-led-blink-millis): the button switches blinking on and off.
2. Move the debounce logic into a function `bool buttonPressed()`.
3. Detect a **long press** (> 1 s) and do something different.

## Further Reading

- [Arduino docs: Debounce](https://docs.arduino.cc/built-in-examples/digital/Debounce/)
- [Jack Ganssle: A Guide to Debouncing](https://www.ganssle.com/debouncing.htm)
