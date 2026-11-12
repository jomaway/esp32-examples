# ESP32 Rotary Encoder

Read a rotary encoder (KY-040) with interrupts and move a light along five LEDs as you turn the knob.

> **Learning path:** 4. External Interrupts and Timers · [← Previous: External Interrupt Button](../esp32-exti-btn) · [Next: Timer LED Blink →](../esp32-timer-led-blink)

## New Concepts

- How a **quadrature encoder** works (two signals A/B, 90° phase shift)
- Decoding with a **lookup table** instead of many `if` statements
- Combining bits into an index: `(prev << 2) | curr`
- **Atomic access** to shared data with `noInterrupts()` / `interrupts()`
- `CHANGE` interrupts on two pins sharing one ISR
- Signed integer arithmetic and handling negative modulo results

## Wiring

| Part              | ESP32 pin              | Notes                       |
| ----------------- | ---------------------- | --------------------------- |
| Encoder CLK (A)   | GPIO 34                |                             |
| Encoder DT (B)    | GPIO 35                |                             |
| Encoder VCC / GND | 3V3 / GND              |                             |
| 5 × LED           | GPIO 19, 18, 5, 17, 16 | each 220 Ω resistor to GND  |

Serial monitor: 115200 baud.

> **Note:** The code uses `INPUT_PULLUP`, but GPIO 34/35 have no internal pull-ups. It works because the KY-040 module has pull-up resistors on board. With a bare encoder, add external pull-ups or use other pins.

## How it works

The A/B signals run through the sequence `00 → 01 → 11 → 10` in one direction and backwards in the other. On every edge the ISR combines the previous and current 2-bit state into a 4-bit index. The lookup table returns `+1`, `-1` or `0` (invalid/bounce). The ISR only accumulates `encDelta`; `loop()` reads and resets it with interrupts disabled so no step gets lost. Each detent of the KY-040 produces 4 steps, hence `encPos / 4`.

## Exercises

1. Use the encoder push button (SW pin) to reset the position to 0.
2. Display the position as a "bar graph" (number of LEDs on).
3. Limit the position to 0–100 and print it as a percentage.

## Further Reading

- [Wikipedia: Incremental encoder](https://en.wikipedia.org/wiki/Incremental_encoder#Quadrature_outputs)
- [Wokwi KY-040 reference](https://docs.wokwi.com/parts/wokwi-ky-040)
