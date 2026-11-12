# ESP32 PWM LED Fade

Smoothly fade an LED in and out using **pulse-width modulation (PWM)**.

> **Learning path:** 5. ADC and PWM · [← Previous: ADC Poti](../esp32-adc-poti) · [Next: PWM Servo →](../esp32-pwm-servo)

## New Concepts

- **PWM**: simulating an "analog" output by switching very fast
- **Frequency**, **duty cycle** and **resolution** (8 bit → duty 0 … 255)
- The ESP32 **LEDC** peripheral: `ledcAttach(pin, freq, resolution)` and `ledcWrite(pin, duty)` (core 3.x API)
- Compile-time version check with `static_assert`

## Wiring

| Part | ESP32 pin | Notes                 |
| ---- | --------- | --------------------- |
| LED  | GPIO 18   | 220 Ω resistor to GND |

Serial monitor: 9600 baud.

## Requirements

Needs **Arduino-ESP32 core 3.x** – the LEDC API changed in 3.0 (`ledcSetup()`/`ledcAttachPin()` were replaced by `ledcAttach()`). Provided by `espressif32 @ ~8.0.0` in `platformio.ini`; the `static_assert` stops the build on older cores.

## How it works

At 5 kHz the LED is switched on and off 5000 times per second – far too fast for the eye. Only the ratio of on-time to period (the duty cycle) is perceived as brightness. The `for` loops increase the duty from 0 to 255 and back, with 10 ms per step.

## Exercises

1. Control the brightness with the potentiometer from [ADC Poti](../esp32-adc-poti).
2. The perceived brightness isn't linear. Apply a gamma correction (e.g. `duty = pow(x / 255.0, 2.2) * 255`).
3. Make the fade non-blocking with `millis()`.

## Further Reading

- [Arduino-ESP32 LEDC API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/ledc.html)
- [Arduino docs: Basics of PWM](https://docs.arduino.cc/learn/microcontrollers/analog-output/)
