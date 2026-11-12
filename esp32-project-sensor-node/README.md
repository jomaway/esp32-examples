# Project: Sensor Node (advanced)

A small weather station: measure temperature, humidity and pressure, show them on the OLED and use an RGB **NeoPixel** as a status light.

> **Learning path:** 7. Buses: I²C and SPI – advanced project · [← Previous: SPI TFT Display](../esp32-spi-tft) · last example

This is a **transfer project**: most of it combines what you already know, but the NeoPixel is new. You add its library yourself, the way you will in your own projects later.

## Requirements

- Read the BME280 every 2 s and print the values over serial.
- The OLED shows **one value at a time**: temperature, humidity or pressure.
- The button switches to the next page. The display updates immediately.
- The NeoPixel shows the temperature: **blue** below 20 °C, **green** from 20 to 25 °C, **red** above 25 °C.
- If no sensor is found, show an error on the display and turn the NeoPixel magenta.
- Nothing blocks: the button must react at any time.

## What You Need

From earlier examples:

- `buttonPressed()` from [Button Toggle with millis()](../esp32-btn-toggle-millis) and a `millis()` timer
- `enum class` and `switch` for the pages, as in [FSM Vending Machine](../esp32-fsm-vending-machine)
- The OLED from [I²C OLED Display](../esp32-i2c-oled) and the `showScreen()` helper from the [vending machine project](../esp32-project-vending-machine)
- The BME280 from [I²C BME280 Sensor](../esp32-i2c-bme280). Both devices share one I²C bus.

New and up to you: the **NeoPixel**.

## Hints for the NeoPixel

A NeoPixel (WS2812B) is an RGB LED with a tiny controller inside. It is controlled through **one data pin** with a special timing protocol, and the library takes care of that.

1. **Find the library.** Search for "NeoPixel" in the PlatformIO library registry (PlatformIO sidebar → *Libraries*) and add the Adafruit library to `lib_deps`, the same way as the display library.
2. **Look at the examples.** Every library comes with examples. The `simple` example of Adafruit NeoPixel shows everything you need.
3. **The pattern is the same as for the display:**
   - create an object: number of LEDs, pin, colour order (`NEO_GRB + NEO_KHZ800`)
   - call `begin()` in `setup()`
   - set a colour with `setPixelColor(index, color)`. Like the OLED, this only changes a buffer
   - send it to the LED with `show()`
4. **Write a helper** like `setNeoPixelColor(red, green, blue)`, so the rest of your code stays readable.

## Wiring

| Part              | ESP32 pin    | Notes                                   |
| ----------------- | ------------ | --------------------------------------- |
| OLED + BME280 SDA | GPIO 21      | 4.7 kΩ pull-up to 3V3                   |
| OLED + BME280 SCL | GPIO 22      | 4.7 kΩ pull-up to 3V3                   |
| Page button       | GPIO 34      | other side to GND, 10 kΩ pull-up to 3V3 |
| NeoPixel DIN      | GPIO 4       | VDD to 3V3, VSS to GND                  |

Serial monitor: 115200 baud.

> **Real hardware:** NeoPixels are specified for 5 V, but a single one usually works at 3.3 V. For LED strips use a 5 V supply and a level shifter for the data line.

## How it works (solution)

`loop()` reacts to two things: a button press switches `currentPage` with `nextPage()`, and the timer triggers `readSensor()`. Both end with `showPage()`, so the display is only redrawn when the page or the values change. `updateStatusColor()` maps the temperature to a colour with an `if` / `else if` chain.

## Exercises

1. Add **hysteresis** to the colour thresholds (as in the [ADC Poti](../esp32-adc-poti) exercise), so the colour doesn't flicker when the temperature is right at 20 °C.
2. Add a fourth page that shows all three values at once in small text.
3. Show a small graph of the last 60 temperature readings (hint: an array and `drawLine()`).
4. Long press on the button: switch the NeoPixel off.
5. Replace the OLED with the colour display from [SPI TFT Display](../esp32-spi-tft) (CS 5, D/C 16, RST 17, so no pin conflicts). Which lines of code really have to change? Use colours for the values.

## Further Reading

- [Adafruit NeoPixel Überguide](https://learn.adafruit.com/adafruit-neopixel-uberguide)
- [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel)
- [PlatformIO library registry](https://registry.platformio.org/)
