# ESP32 SPI TFT Display

Draw on a 320×240 colour display (**ILI9341**) over **SPI**, and measure how fast a full screen is transferred.

> **Learning path:** 7. Buses: I²C and SPI · [← Previous: I²C BME280 Sensor](../esp32-i2c-bme280) · [Next: Project Sensor Node →](../esp32-project-sensor-node)

## New Concepts

- The **SPI bus**: MOSI, MISO, SCK and one **chip select (CS)** line per device
- I²C vs. SPI: selecting devices by address vs. by CS line, speed, number of wires
- The **D/C pin** of displays: are the bytes a command or pixel data?
- **RGB565** colours: 16 bit per pixel, `color565(r, g, b)`
- Drawing **without a buffer**: every call goes straight to the display
- `micros()` to measure short durations
- Same drawing library, different display: the **Adafruit GFX** functions you know from the [OLED](../esp32-i2c-oled)

## Libraries

```ini
lib_deps =
	adafruit/Adafruit ILI9341@^1.6.1
	adafruit/Adafruit GFX Library@^1.12.1
```

## Wiring

| Display pin | ESP32 pin | Notes                                     |
| ----------- | --------- | ----------------------------------------- |
| VCC         | 3V3       |                                           |
| GND         | GND       |                                           |
| CS          | GPIO 5    | chip select (default SS pin)              |
| RST         | GPIO 17   | reset                                     |
| D/C         | GPIO 16   | data / command                            |
| MOSI        | GPIO 23   | controller → display (default MOSI)       |
| SCK         | GPIO 18   | clock (default SCK)                       |
| LED         | 3V3       | backlight                                 |
| MISO        | GPIO 19   | display → controller (default MISO), optional for this example |

Serial monitor: 115200 baud.

## I²C vs. SPI

|                     | I²C                                  | SPI                                          |
| ------------------- | ------------------------------------ | -------------------------------------------- |
| Wires               | 2 (SDA, SCL), no matter how many devices | 3 shared (MOSI, MISO, SCK) + **1 CS per device** |
| Selecting a device  | by **address** in the data stream    | by pulling its **CS** line LOW               |
| Direction           | one data line, takes turns           | separate lines, both directions at once      |
| Typical speed       | 100 kHz – 400 kHz (up to 1 MHz)      | 10 – 80 MHz                                  |
| Good for            | sensors, small displays, many devices with few wires | large displays, SD cards, fast data |

## How it works

A full screen is 320 × 240 pixels × 2 bytes = **153 600 bytes**, 150 times more than the 1 KB of the OLED. At 400 kHz, I²C would need about 4 seconds for this. SPI at 40 MHz does it in a few dozen milliseconds. `drawSpeedTest()` measures it:

```cpp
uint32_t start = micros();
tft.fillScreen(colors[colorIndex]);
uint32_t duration = micros() - start;
```

The drawing functions are the same as for the OLED (`setCursor()`, `print()`, `fillRect()` …), because both libraries are built on Adafruit GFX. The big difference: the ILI9341 library has **no buffer**. There is no `display()` call, and every function sends its pixels immediately. To change a text you must overwrite it, for example with `setTextColor(foreground, background)`.

> **Note:** In the Wokwi simulation the measured times are not realistic. Try it on real hardware for real numbers.

## Exercises

1. Change `spiFrequency` to 10 MHz and 80 MHz. How does the measured time change?
2. Show a counter that updates every second without clearing the whole screen. (Hint: `setTextColor(ILI9341_WHITE, ILI9341_BLACK)` draws the text with a background.)
3. Calculate how long the full screen would take over I²C at 400 kHz (each byte needs 9 clock pulses including the ACK) and print it next to the measured SPI time.

## Further Reading

- [SparkFun: SPI tutorial](https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi)
- [Adafruit ILI9341 library](https://github.com/adafruit/Adafruit_ILI9341)
- [Adafruit GFX graphics library guide](https://learn.adafruit.com/adafruit-gfx-graphics-library)
- [Wokwi ILI9341 reference](https://docs.wokwi.com/parts/wokwi-ili9341)
