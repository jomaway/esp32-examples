# ESP32 I²C OLED Display

Show text and a progress bar on a 128×64 pixel **SSD1306 OLED** display, your first external library.

> **Learning path:** 7. Buses: I²C and SPI · [← Previous: I²C Scanner](../esp32-i2c-scanner) · [Next: Project Vending Machine with Display →](../esp32-project-vending-machine)

## New Concepts

- Using **external libraries**: `lib_deps` in `platformio.ini`
- Library **objects**: create one (`Adafruit_SSD1306 display(...)`), start it with `begin()`, then call its methods
- **Frame buffer**: drawing happens in RAM. `display.display()` sends the whole picture to the display
- Pixel coordinates: `(0, 0)` is the top left corner
- Text (`setTextSize()`, `setCursor()`, `print()`) and shapes (`drawRect()`, `fillRect()`, `drawLine()`)
- Stopping the program on a fatal error (`while (true)`)

## Libraries

```ini
lib_deps =
	adafruit/Adafruit SSD1306@^2.5.13
	adafruit/Adafruit GFX Library@^1.12.1
```

PlatformIO downloads them automatically on the first build. **Adafruit GFX** provides all drawing functions (text, lines, rectangles …) and is shared by many Adafruit displays. **Adafruit SSD1306** knows how to talk to this specific display.

## Wiring

| Part      | ESP32 pin | Notes                  |
| --------- | --------- | ---------------------- |
| OLED SDA  | GPIO 21   | 4.7 kΩ pull-up to 3V3  |
| OLED SCL  | GPIO 22   | 4.7 kΩ pull-up to 3V3  |
| OLED VCC  | 3V3       |                        |
| OLED GND  | GND       |                        |

Serial monitor: 115200 baud. `display.begin()` also starts the I²C bus on the default pins (21/22).

## How it works

Every drawing call only changes a **buffer** in the ESP32's RAM: 128 × 64 pixels = 8192 bits = 1 KB. The display shows nothing new until you send the buffer:

```cpp
display.clearDisplay();      // 1. clear the buffer
display.println("Hello!");   // 2. draw into the buffer
display.drawRect(...);
display.display();           // 3. send 1 KB over I²C -> now you see it
```

Sending 1 KB over I²C takes a few milliseconds. That's why the program only redraws when something changed (once per second here), not on every `loop()` run.

## Exercises

1. Draw a frame around the whole screen and center the text "Hello!".
2. Show the value of the potentiometer from [ADC Poti](../esp32-adc-poti) as a number and as a bar.
3. Count how many times `loop()` runs per second and print the number. Then call `drawScreen()` on every `loop()` run without the timer. How much slower does the loop get?

## Further Reading

- [Adafruit GFX graphics library guide](https://learn.adafruit.com/adafruit-gfx-graphics-library)
- [Adafruit SSD1306 library](https://github.com/adafruit/Adafruit_SSD1306)
- [PlatformIO: `lib_deps`](https://docs.platformio.org/en/latest/projectconf/sections/env/options/library/lib_deps.html)
- [Wokwi SSD1306 reference](https://docs.wokwi.com/parts/board-ssd1306)
