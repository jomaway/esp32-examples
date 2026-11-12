# ESP32 Button Press

Read a push button: the LED is on as long as the button is pressed.

> **Learning path:** 1. GPIOs and Serial Output · [← Previous: Serial Debug](../esp32-serial-debug) · [Next: Button Toggle →](../esp32-btn-toggle)

## New Concepts

- Digital input with `pinMode(pin, INPUT)` and `digitalRead(pin)`
- **Pull-up resistor** – defines the input level while the button is open
- **Active-low** logic: pressed = `LOW`, released = `HIGH`
- Decisions with `if` / `else`

## Wiring

| Part        | ESP32 pin | Notes                                            |
| ----------- | --------- | ------------------------------------------------ |
| LED         | GPIO 18   | through a 220 Ω resistor to GND                  |
| Push button | GPIO 34   | other side to GND                                |
| Pull-up     | GPIO 34   | 10 kΩ resistor from GPIO 34 to 3V3               |

## How it works

The pull-up resistor pulls GPIO 34 to `HIGH` while the button is open. Pressing the button connects the pin directly to GND, so it reads `LOW`. The `loop()` reads the pin continuously and mirrors its state on the LED.

> **Why an external resistor?** Most ESP32 pins have an internal pull-up you can enable with `INPUT_PULLUP`. GPIO 34–39 are input-only pins **without** internal pull-ups, so they always need an external resistor.

## Exercises

1. Invert the behaviour: LED off while the button is pressed.
2. Move the button to a pin with an internal pull-up (e.g. GPIO 25), remove the external resistor and use `INPUT_PULLUP`.
3. Add a second button and LED.

## Further Reading

- [Arduino reference: `digitalRead()`](https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalread/)
- [Arduino-ESP32 GPIO API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/gpio.html)
- [SparkFun: Pull-up resistors](https://learn.sparkfun.com/tutorials/pull-up-resistors)
