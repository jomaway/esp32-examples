# ESP32 ADC Potentiometer

Read an analog voltage from a potentiometer and switch an LED when it crosses a threshold.

> **Learning path:** 5. ADC and PWM · [← Previous: Timer LED Blink](../esp32-timer-led-blink) · [Next: PWM LED Fade →](../esp32-pwm-led-fade)

## New Concepts

- **Analog-to-digital converter (ADC)**: turning a voltage into a number
- `analogRead(pin)` – no `pinMode()` needed
- **ADC resolution** with `analogReadResolution(bits)`: 10 bit → 0 … 1023 (default: 12 bit → 0 … 4095)
- **Voltage divider** – how a potentiometer works
- Threshold comparison

## Wiring

| Part               | ESP32 pin | Notes                  |
| ------------------ | --------- | ---------------------- |
| LED                | GPIO 18   | 220 Ω resistor to GND  |
| Potentiometer SIG  | GPIO 35   | wiper                  |
| Potentiometer VCC  | 3V3       |                        |
| Potentiometer GND  | GND       |                        |

Serial monitor: 9600 baud.

> **Note:** Use ADC1 pins (GPIO 32–39) for analog input. ADC2 pins can't be used while Wi-Fi is active.

## How it works

The potentiometer divides 3.3 V depending on the knob position. `analogRead()` converts this voltage into a value between 0 and 1023. If the value is ≥ 500 the LED turns on. A `delay(100)` limits the output to 10 lines per second so the serial monitor stays readable.

## Exercises

1. Convert the raw value into a voltage and print it (`value * 3.3 / 1023`).
2. Add a **hysteresis** (on at > 550, off at < 450) and observe the difference near the threshold.
3. Show the value as a bar graph on the 5 LEDs from [LED Array](../esp32-led-array).

## Further Reading

- [Arduino-ESP32 ADC API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html)
- [Arduino reference: `analogRead()`](https://docs.arduino.cc/language-reference/en/functions/analog-io/analogRead/)
