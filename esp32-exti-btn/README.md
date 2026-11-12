# ESP32 External Interrupt Button

React to button presses with **external interrupts** – even while the `loop()` is busy and never looks at the buttons.

> **Learning path:** 4. External Interrupts and Timers · [← Previous: FSM Vending Machine](../esp32-fsm-vending-machine) · [Next: Rotary Encoder →](../esp32-exti-encoder)

## New Concepts

- **Interrupts** and **Interrupt Service Routines (ISR)**
- `attachInterrupt(pin, isr, FALLING)` and trigger modes (`RISING`, `FALLING`, `CHANGE`)
- `IRAM_ATTR` – places the ISR in internal RAM (required on the ESP32)
- `volatile` for variables shared between ISR and main program
- Rule of thumb: **keep ISRs short** – just set a flag

## Wiring

| Part          | ESP32 pin | Notes                                    |
| ------------- | --------- | ---------------------------------------- |
| LED           | GPIO 18   | 220 Ω resistor to GND                    |
| Start button  | GPIO 34   | other side to GND, 10 kΩ pull-up to 3V3  |
| Stop button   | GPIO 35   | other side to GND, 10 kΩ pull-up to 3V3  |

Serial monitor: 115200 baud.

## How it works

While `running` is `true`, the `loop()` is stuck in a `while` loop doing "work" (toggling the LED, printing dots). It never reads the buttons. Still, pressing *Stop* works: the hardware interrupt pauses the program, `stopISR()` sets `running = false`, and the `while` loop ends at its next check.

The file contains two variants – switch between them in `loop()`:

- `loop_example_1()` – work with `delay(2000)`: *Stop* is noticed only after the delay finishes.
- `loop_example_2()` – work with `millis()`: *Stop* is noticed almost instantly.

## Exercises

1. Remove `volatile` and compile with optimisation. Does it still work? (It may – or may not. That's the point.)
2. Count button presses inside an ISR and print the count in `loop()`. Do you see bouncing?
3. Use a single button with `CHANGE` to start while pressed and stop when released.

## Further Reading

- [Arduino reference: `attachInterrupt()`](https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/)
- [Arduino-ESP32 GPIO API (interrupts)](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/gpio.html)
