# ESP32 Button Toggle

Each press of the button toggles the LED: press once → on, press again → off.

> **Learning path:** 1. GPIOs and Serial Output · [← Previous: Button Press](../esp32-btn-press) · [Next: LED Blink with millis() →](../esp32-led-blink-millis)

## New Concepts

- **Edge detection**: reacting to the *change* of a signal (falling edge `HIGH → LOW`) instead of its level
- Remembering state between `loop()` runs with **global variables** (`lastButtonState`, `ledState`)
- **Button bouncing** and simple **debouncing** with a short `delay()`
- Toggling a `bool` with the `!` operator

## Wiring

Same circuit as [Button Press](../esp32-btn-press):

| Part        | ESP32 pin | Notes                               |
| ----------- | --------- | ----------------------------------- |
| LED         | GPIO 18   | through a 220 Ω resistor to GND     |
| Push button | GPIO 34   | other side to GND                   |
| Pull-up     | GPIO 34   | 1 kΩ resistor to 3V3                |

## How it works

The program compares the current button reading with the reading from the previous loop. A falling edge means the button was just pressed. Mechanical contacts "bounce" for a few milliseconds and create several edges, so the program waits 50 ms and checks again before toggling the LED.

> **Downside:** `delay()` blocks the whole program for 50 ms. The next section shows how to debounce without blocking.

## Exercises

1. Remove the debounce `delay()` and try it on real hardware. What happens?
2. Toggle on the **rising** edge (button released) instead.
3. Count the button presses and print the count over serial.

## Further Reading

- [Arduino docs: Debounce](https://docs.arduino.cc/built-in-examples/digital/Debounce/)
- [Jack Ganssle: A Guide to Debouncing](https://www.ganssle.com/debouncing.htm)
