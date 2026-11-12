# ESP32 Serial Debug

Send text messages from the ESP32 to your PC. This is the simplest and most important debugging tool: print what your program is doing.

> **Learning path:** 1. GPIOs and Serial Output · [← Previous: LED Array](../esp32-led-array) · [Next: Button Press →](../esp32-btn-press)

## New Concepts

- Starting the serial interface with `Serial.begin(baudrate)`
- Printing text with `Serial.println()`
- Opening the **Serial Monitor** (PlatformIO: plug icon in the status bar; Wokwi: opens automatically)
- The **baud rate** – PC and ESP32 must use the same speed (here `9600`)
- Reading a digital input with `pinMode(pin, INPUT)` and `digitalRead()` (preview of the next example)

## Wiring

| Part        | ESP32 pin | Notes                                    |
| ----------- | --------- | ---------------------------------------- |
| LED         | GPIO 18   | through a 220 Ω resistor to GND          |
| Push button | GPIO 34   | other side to GND (active low)           |
| Pull-up     | GPIO 34   | 1 kΩ resistor to 3V3                     |

## How it works

Every 250 ms the program reads the button and prints whether it is pressed or released, and switches the LED accordingly. Without the `delay(250)` the serial monitor would be flooded with thousands of lines per second.

## Exercises

1. Change the baud rate in the code to `115200`. What happens in the monitor? Fix it by setting `monitor_speed = 115200` in `platformio.ini`.
2. Only print a message when the button state **changes** instead of every 250 ms.
3. Print a counter value together with each message using `Serial.print()` and `Serial.println()`.

## Further Reading

- [Arduino reference: `Serial`](https://docs.arduino.cc/language-reference/en/functions/communication/serial/)
- [PlatformIO: `monitor_speed`](https://docs.platformio.org/en/latest/projectconf/sections/env/options/monitor/monitor_speed.html)
