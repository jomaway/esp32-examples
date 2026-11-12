# ESP32 UART Byte Input

Control an LED by sending single characters from the PC to the ESP32.

> **Learning path:** 6. Communication Protocols · [← Previous: PWM Servo](../esp32-pwm-servo) · [Next: UART String Input →](../esp32-uart-string-input)

## New Concepts

- **Receiving** data over UART (the serial monitor works in both directions)
- `Serial.available()` and `Serial.read()`
- Characters are just bytes: **ASCII** (`'o'` == `0x6F`)
- `switch` on received bytes

## Wiring

| Part | ESP32 pin | Notes                 |
| ---- | --------- | --------------------- |
| LED  | GPIO 18   | 220 Ω resistor to GND |

Serial monitor: 115200 baud.

## Usage

Type a character into the serial monitor and send it:

| Byte | Action      |
| ---- | ----------- |
| `o`  | LED on      |
| `r`  | LED off     |
| `t`  | toggle LED  |

> **Tip:** Many serial monitors append a line ending (`\r\n`) when you hit Enter. These bytes produce "Invalid byte received". Set the line ending to *None*, or ignore `\r` and `\n` in the code.

## Exercises

1. Ignore `'\r'` and `'\n'` so no error is printed.
2. Uncomment the debug line to see every received byte in hex.
3. Send a digit `'0'`–`'9'` to set the blink speed of the LED.

## Further Reading

- [Arduino reference: `Serial.read()`](https://docs.arduino.cc/language-reference/en/functions/communication/serial/read/)
- [ASCII table](https://www.asciitable.com/)
