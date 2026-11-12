# Project: Vending Machine with Display

Give the [FSM Vending Machine](../esp32-fsm-vending-machine) a user interface: an OLED display that tells the customer what to do.

> **Learning path:** 7. Buses: I²C and SPI – project · [← Previous: I²C OLED Display](../esp32-i2c-oled) · [Next: I²C BME280 Sensor →](../esp32-i2c-bme280)

## Requirements

The machine behaves exactly like the [FSM Vending Machine](../esp32-fsm-vending-machine) (same buttons, LEDs and states). In addition, the display shows:

| State          | Display                         |
| -------------- | ------------------------------- |
| `Idle`         | "Ready" / "Insert coin"         |
| `CoinInserted` | the coin count, e.g. "1 / 2"    |
| `Dispensing`   | "Please wait" / "Dispensing"    |
| `OutOfOrder`   | "Error" / "Out of order"        |

- The coin count updates with every inserted coin.
- The display is only redrawn when something changes.

## What You Need

No new concepts. The project only combines what you already know:

- The state machine from [FSM Vending Machine](../esp32-fsm-vending-machine) (states, events, guards, `struct Button`)
- The OLED display from [I²C OLED Display](../esp32-i2c-oled) (`lib_deps`, buffer + `display()`)
- `String` and `+` to build texts from [UART String Input](../esp32-uart-string-input)

## Wiring

The circuit of the FSM vending machine plus the OLED display:

| Part                  | ESP32 pin | Wokwi key | Notes                       |
| --------------------- | --------- | --------- | --------------------------- |
| "Insert Coin" button  | GPIO 34   | `Space`   | 10 kΩ pull-up, active low   |
| "Select Item" button  | GPIO 35   | `s`       | 10 kΩ pull-up, active low   |
| "Reset" button        | GPIO 27   | `r`       | 10 kΩ pull-up, active low   |
| Error LED (red)       | GPIO 19   |           | 220 Ω resistor to GND       |
| Ready LED (green)     | GPIO 18   |           | 220 Ω resistor to GND       |
| OLED SDA / SCL        | GPIO 21 / 22 |        | 4.7 kΩ pull-ups to 3V3      |

Serial monitor: 115200 baud.

## How it works

The state machine is unchanged. Each state's **entry action** in `enterState()` now also draws its screen, so the display changes exactly when the state changes:

```cpp
case VendingState::OutOfOrder:
  ...
  showScreen("Error", "Out of\norder");
  break;
```

`showScreen(title, text)` is a small helper that clears the buffer, draws a title line and a big text, and sends it to the display.

The only screen update *without* a state change is the coin count: another coin in `CoinInserted` is an internal transition, which calls `showCoins()` directly.

## Things to Think About

Imagine the display were updated in `loop()` on every run instead. It would send 1 KB over I²C thousands of times per second, the buttons would react more slowly, and you would need extra variables to remember what is already on the screen. With an FSM the question *"when do I redraw?"* has a simple answer: **when entering a state**.

## Exercises

1. Show "Need 2 coins" for one second when *Select Item* is pressed with too few coins, then return to the coin screen. (Hint: a new state or the timer.)
2. Draw a progress bar while dispensing (update it a few times during the 2 s).
3. Add a second item with a different price and show both prices on the `Idle` screen.
