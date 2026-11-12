# ESP32 Button Toggle with `millis()`

Toggle an LED with a button – this time with **non-blocking debouncing** based on `millis()` instead of `delay()`.

> **Learning path:** 2. Non-blocking GPIO usage · [← Previous: LED Blink with millis()](../esp32-led-blink-millis) · [Next: Button Toggle with Shift Register →](../esp32-btn-toggle-shift)

## New Concepts

- Debouncing without blocking: a signal is only accepted once it has been **stable** for a certain time
- Separating the *raw* state (`lastButtonState`) from the *debounced* state (`lastStableButtonState`)
- A function that **returns a value**: `bool buttonPressed()` – return type, `return`, calling it inside an `if`
- Separating *what* the program does (`loop()`) from *how* it is done (`buttonPressed()`)

## Wiring

| Part        | ESP32 pin | Notes                             |
| ----------- | --------- | --------------------------------- |
| LED         | GPIO 18   | through a 220 Ω resistor to GND   |
| Push button | GPIO 34   | other side to GND                 |
| Pull-up     | GPIO 34   | 10 kΩ resistor to 3V3             |

## How it works

1. Every change of the raw button reading (also bounces) restarts the timer `lastChangeTime`.
2. Only when the reading hasn't changed for 50 ms is it treated as *stable*.
3. A falling edge of the **stable** state (`HIGH → LOW`) toggles the LED.

All of this lives in `buttonPressed()`, which returns `true` exactly once per press. The function only *detects* the press – it doesn't touch the LED. That keeps `loop()` short and readable:

```cpp
void loop()
{
  if (buttonPressed())
  {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}
```

Because nothing waits, `buttonPressed()` must be called on every `loop()` run, and other tasks keep running smoothly.

> **Why only one `return`?** An early `return true;` inside the `if` would skip the line `lastButtonState = currentButtonState;` and break the edge detection. Collecting the result in `pressed` and returning once at the end makes sure the state is always updated.

## Things to Think About

`buttonPressed()` keeps its state in global variables and reads a fixed pin, so it only works for **one** button. How would you debounce two buttons? Later examples show two answers: passing the state by reference ([FSM Vending Machine](../esp32-fsm-vending-machine)) and wrapping it in a class ([Component Drivers](../esp32-component-drivers)).

## Exercises

1. Combine this example with [LED Blink with millis()](../esp32-led-blink-millis): the button switches blinking on and off.
2. Write a function `void toggleLed()` (a function **without** a return value) and use it in `loop()`.
3. Detect a **long press** (> 1 s) and do something different.

## Further Reading

- [Arduino docs: Debounce](https://docs.arduino.cc/built-in-examples/digital/Debounce/)
- [Jack Ganssle: A Guide to Debouncing](https://www.ganssle.com/debouncing.htm)
