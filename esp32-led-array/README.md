# ESP32 LED Array

Control five LEDs at once to build a simple "running light": the LEDs switch on one after the other and then off again in reverse order.

> **Learning path:** 1. GPIOs and Serial Output · [← Previous: LED Blink](../esp32-led-blink) · [Next: Serial Debug →](../esp32-serial-debug)

## New Concepts

- Storing pin numbers in a `constexpr` **array**
- Iterating over the array with a `for` loop (forwards and backwards)
- Using a constant (`ledCount`) for the array size instead of a "magic number"

## Wiring

| Part      | ESP32 pin                  | Notes                                   |
| --------- | -------------------------- | --------------------------------------- |
| 5 × LED   | GPIO 19, 18, 5, 17, 16     | each through a 220 Ω resistor to GND    |

## How it works

All pins are stored in `ledPins[]`. In `setup()` a `for` loop sets every pin to `OUTPUT`. In `loop()` a first `for` loop turns the LEDs on from index `0` to `4`, a second one turns them off from index `4` back to `0`, with 300 ms between each step.

Adding a sixth LED only needs a new pin in the array and an updated `ledCount` – the rest of the code stays the same.

## Exercises

1. Build a "Knight Rider" light: only **one** LED is on at a time and the light runs back and forth.
2. Let all LEDs blink at the same time.
3. Show a 5-bit binary counter (0–31) on the LEDs.

## Further Reading

- [C++ arrays (cppreference)](https://en.cppreference.com/w/cpp/language/array)
- [Arduino reference: `for`](https://docs.arduino.cc/language-reference/en/structure/control-structure/for/)
