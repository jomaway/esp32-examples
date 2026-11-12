# ESP32 UART String Input

A tiny command-line interface: control an LED with text commands like `led on`.

> **Learning path:** 6. Communication Protocols · [← Previous: UART Byte Input](../esp32-uart-byte-input) · [Next: Sensor Node →](../esp32-sensor-node)

## New Concepts

- Reading a full line with `Serial.readStringUntil('\n')`
- The Arduino `String` class: `trim()`, `toLowerCase()`, `equals()`
- Parsing simple text commands
- Status replies with `Serial.printf()` and the ternary operator `cond ? a : b`

## Wiring

| Part | ESP32 pin | Notes                 |
| ---- | --------- | --------------------- |
| LED  | GPIO 18   | 220 Ω resistor to GND |

Serial monitor: 115200 baud, line ending *Newline* (`\n`).

## Usage

| Command       | Action                       |
| ------------- | ---------------------------- |
| `led on`      | turn LED on                  |
| `led off`     | turn LED off                 |
| `led toggle`  | toggle LED                   |
| `led status`  | print current LED state      |
| `help`        | show available commands      |

Commands are case-insensitive; leading/trailing spaces are ignored.

> **Note:** `readStringUntil()` blocks until it receives `\n` or hits the timeout (default 1 s, change with `Serial.setTimeout()`).

## Exercises

1. Add a command `led blink <ms>` that parses a number (`substring()`, `toInt()`).
2. Read characters into a buffer yourself so the loop never blocks.
3. Control the servo from [PWM Servo](../esp32-pwm-servo) with `servo <angle>`.

## Further Reading

- [Arduino reference: `String`](https://docs.arduino.cc/language-reference/en/variables/data-types/stringObject/)
- [Arduino reference: `Serial.readStringUntil()`](https://docs.arduino.cc/language-reference/en/functions/communication/serial/readStringUntil/)
