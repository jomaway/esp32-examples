# ESP32 Button Toggle with a Shift Register

A third way to debounce a button: sample it every 5 ms and shift each reading into an 8-bit "history" variable. The button only counts as pressed or released when the history shows a stable pattern.

> **Learning path:** 2. Non-blocking GPIO usage · [← Previous: Button Toggle with millis()](../esp32-btn-toggle-millis) · [Next: Project Traffic Light →](../esp32-project-traffic-light)

## New Concepts

- **Periodic sampling** with `millis()`
- **Bit manipulation**: left shift `<<`, bitwise OR `|`, bitwise AND `&`
- **Bit masks** and hexadecimal literals (`0x07`, `0xFF`)
- Software "shift register" as a debounce filter

## Wiring

| Part        | ESP32 pin | Notes                             |
| ----------- | --------- | --------------------------------- |
| LED         | GPIO 18   | through a 220 Ω resistor to GND   |
| Push button | GPIO 34   | other side to GND                 |
| Pull-up     | GPIO 34   | 10 kΩ resistor to 3V3             |

Serial monitor: 9600 baud.

## How it works

```cpp
buttonHistory = (buttonHistory << 1) | currentButtonState;
```

Every 5 ms all bits move one position to the left and the newest reading enters at bit 0. The variable therefore holds the last 8 readings:

| `buttonHistory` | Meaning                                         |
| --------------- | ----------------------------------------------- |
| `0bxxxxx000`    | last 3 samples LOW → button **pressed**         |
| `0b11111111`    | last 8 samples HIGH → button **released**       |
| anything else   | bouncing – keep the previous stable state       |

Using a short window for "pressed" and a long one for "released" gives a fast response and a reliable release.

## Exercises

1. Change the masks/sample time. How does that change the debounce time?
2. Debounce two buttons using two history variables.
3. Print `buttonHistory` in binary with `Serial.println(buttonHistory, BIN)` to watch the bits move.

## Further Reading

- [Bitwise operators (cppreference)](https://en.cppreference.com/w/cpp/language/operator_arithmetic#Bitwise_logic_operators)
- [Jack Ganssle: A Guide to Debouncing](https://www.ganssle.com/debouncing.htm)
