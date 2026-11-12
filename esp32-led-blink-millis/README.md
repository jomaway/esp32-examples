# ESP32 LED Blink with `millis()`

Blink an LED **without** `delay()`. The `loop()` never blocks, so the program could do other things at the same time.

> **Learning path:** 2. Non-blocking GPIO usage · [← Previous: Button Toggle](../esp32-btn-toggle) · [Next: Button Toggle with millis() →](../esp32-btn-toggle-millis)

## New Concepts

- **Blocking vs. non-blocking** code
- `millis()` – milliseconds since the board started
- The "has enough time passed?" pattern: `if (now - lastChangeTime >= interval)`
- Storing timestamps in unsigned integer types

## Wiring

| Part        | ESP32 pin | Notes                             |
| ----------- | --------- | --------------------------------- |
| LED (green) | GPIO 18   | through a 220 Ω resistor to GND   |

## How it works

Each loop reads the current time. When at least 300 ms have passed since the last toggle, the LED state is inverted and the current time is stored in `lastChangeTime`. Otherwise `loop()` returns immediately – it runs thousands of times per second instead of sleeping.

```cpp
if (currentMillis - lastChangeTime >= blinkFrequency) { ... }
```

> **Tip:** Always write the comparison as a *subtraction* like above. It keeps working when the `millis()` counter overflows; `millis() >= nextTime` does not.

## Exercises

1. Add a second LED that blinks at a different rate (e.g. 700 ms). Try the same with `delay()` – why is that much harder?
2. Make the on-time and off-time different (100 ms on, 900 ms off).
3. Combine this example with a button that is read in the same `loop()`.

## Further Reading

- [Arduino reference: `millis()`](https://docs.arduino.cc/language-reference/en/functions/time/millis/)
- [Arduino docs: Blink Without Delay](https://docs.arduino.cc/built-in-examples/digital/BlinkWithoutDelay/)
