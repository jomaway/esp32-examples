# ESP32 LED Blink

The "hello world" of microcontrollers: make an LED blink every 500 ms.

> **Learning path:** 1. GPIOs and Serial Output · first example · [Next: LED Array →](../esp32-led-array)

## New Concepts

- The `setup()` and `loop()` functions of an Arduino program
- Including header files with `#include <Arduino.h>`
- `constexpr` compile-time constants as a modern C++ alternative to `#define`
- Fixed-width integer types like `uint8_t`
- Digital output with `pinMode(pin, OUTPUT)` and `digitalWrite(pin, HIGH/LOW)`
- Waiting with `delay(ms)`

## Wiring

| Part            | ESP32 pin | Notes                                  |
| --------------- | --------- | -------------------------------------- |
| LED (green)     | GPIO 18   | through a 220 Ω resistor, cathode to GND |

## How it works

`setup()` runs once and configures GPIO 18 as an output. `loop()` runs forever: it switches the LED on, waits 500 ms, switches it off and waits again.

> **Why the resistor?** An LED has almost no resistance once it conducts. Without a series resistor the current would be far too high for the LED and the GPIO pin.

## Exercises

1. Change the blink frequency. What is the fastest blink you can still see?
2. Make the LED blink in an SOS pattern (`... --- ...`).
3. Create a traffic light with three LEDs (red, yellow, green).
4. Move the blink code into a function `void blink(uint8_t pin)` and use it for **two** LEDs. Do they blink at the same time? Why not? (The answer leads to [LED Blink with millis()](../esp32-led-blink-millis).)

## Further Reading

- [Arduino reference: `pinMode()`](https://docs.arduino.cc/language-reference/en/functions/digital-io/pinMode/)
- [Arduino reference: `digitalWrite()`](https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalwrite/)
- [Arduino reference: `delay()`](https://docs.arduino.cc/language-reference/en/functions/time/delay/)
