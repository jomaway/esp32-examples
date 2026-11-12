# ESP32 Timer LED Blink

Blink an LED using a **hardware timer** interrupt. The `loop()` is completely empty!

> **Learning path:** 4. External Interrupts and Timers · [← Previous: Rotary Encoder](../esp32-exti-encoder) · [Next: ADC Poti →](../esp32-adc-poti)

## New Concepts

- **Hardware timers**: counters that run independently of the CPU
- Timer frequency, ticks and alarm value: `500 000 ticks / 1 MHz = 0.5 s`
- Timer API of Arduino-ESP32 core 3.x: `timerBegin()`, `timerAttachInterrupt()`, `timerAlarm()`
- Pointer types and `nullptr` (`hw_timer_t *myTimer`)
- Compile-time checks with `static_assert`

## Wiring

| Part | ESP32 pin | Notes                 |
| ---- | --------- | --------------------- |
| LED  | GPIO 19   | 220 Ω resistor to GND |

Serial monitor: 115200 baud.

## Requirements

This example needs **Arduino-ESP32 core 3.x**. The timer API changed completely between 2.x and 3.x. `platformio.ini` pins `espressif32 @ ~8.0.0`, which ships core 3.x. The `static_assert` stops the build with a clear message if an older core is used anyway.

## How it works

`timerBegin(1000000)` starts a timer that counts at 1 MHz. `timerAlarm(myTimer, 500000, true, 0)` triggers an interrupt every 500 000 ticks and reloads automatically (`true`). The ISR `blink()` toggles the LED. The timing is exact and independent of what `loop()` does.

## Exercises

1. Make the LED blink at 5 Hz.
2. Add a `delay(3000)` in `loop()`. Does the blinking change? Compare with [LED Blink with millis()](../esp32-led-blink-millis).
3. Count the timer ticks in a `volatile` variable and print elapsed seconds in `loop()`.

## Further Reading

- [Arduino-ESP32 Timer API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/timer.html)
- [Migration guide 2.x → 3.0](https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html)
