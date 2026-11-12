# ESP32 PWM Servo

Control the angle of a hobby servo motor with a potentiometer.

> **Learning path:** 5. ADC and PWM · [← Previous: PWM LED Fade](../esp32-pwm-led-fade) · [Next: UART Byte Input →](../esp32-uart-byte-input)

## New Concepts

- How a **servo** is controlled: 50 Hz signal (20 ms period), pulse width 0.5 ms – 2.5 ms sets the angle
- Calculating duty values for a given resolution (12 bit → 0 … 4095)
- Scaling values with `map(value, fromLow, fromHigh, toLow, toHigh)`
- Combining an input (ADC) with an output (PWM)

## Wiring

| Part              | ESP32 pin | Notes             |
| ----------------- | --------- | ----------------- |
| Servo PWM         | GPIO 18   |                   |
| Servo V+ / GND    | 3V3 / GND | see note below    |
| Potentiometer SIG | GPIO 35   |                   |
| Potentiometer VCC / GND | 3V3 / GND |           |

> **Note:** In Wokwi the servo runs from 3V3. Real servos usually need **5 V** and can draw more current than the board can provide – use a separate supply and connect the grounds.

## How it works

| Angle | Pulse  | Duty cycle | 12-bit value |
| ----- | ------ | ---------- | ------------ |
| 0°    | 0.5 ms | 2.5 %      | 102          |
| 180°  | 2.5 ms | 12.5 %     | 512          |

The potentiometer value (0 … 4095) is mapped to 102 … 512 and written to the LEDC channel.

## Requirements

Needs **Arduino-ESP32 core 3.x** (`ledcAttach()` API), provided by `espressif32 @ ~8.0.0` in `platformio.ini` and checked with `static_assert`.

## Exercises

1. Let the servo sweep automatically from 0° to 180° and back.
2. Write a function `setAngle(uint8_t degrees)`.
3. Set the angle via the serial monitor (see section 6).

## Further Reading

- [Arduino-ESP32 LEDC API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/ledc.html)
- [Arduino reference: `map()`](https://docs.arduino.cc/language-reference/en/functions/math/map/)
- [Wokwi servo reference](https://docs.wokwi.com/parts/wokwi-servo)
