# ESP32 I²C BME280 Sensor

Read temperature, humidity and air pressure from a **BME280** sensor over I²C.

> **Learning path:** 7. Buses: I²C and SPI · [← Previous: Project Vending Machine with Display](../esp32-project-vending-machine) · [Next: SPI TFT Display →](../esp32-spi-tft)

## New Concepts

- Reading a **sensor** with a library: `bme.begin(address)`, `readTemperature()`, `readHumidity()`, `readPressure()`
- Checking the return value of `begin()`: is the sensor really there?
- Devices with **selectable addresses** (`0x76` / `0x77`)
- `float` values and **units**: Pa → hPa
- Printing floats with `printf("%.1f")`
- A Wokwi **custom chip**: a simulated sensor written in C

## Libraries

```ini
lib_deps =
	adafruit/Adafruit BME280 Library@^2.3.0
```

The library pulls in *Adafruit Unified Sensor* automatically.

## Wiring

| Part        | ESP32 pin | Notes                  |
| ----------- | --------- | ---------------------- |
| BME280 SDA  | GPIO 21   | 4.7 kΩ pull-up to 3V3  |
| BME280 SCL  | GPIO 22   | 4.7 kΩ pull-up to 3V3  |
| BME280 VCC  | 3V3       |                        |
| BME280 GND  | GND       |                        |

Serial monitor: 115200 baud.

## How it works

`bme.begin()` checks the sensor's chip ID and reads its **calibration data**. Every BME280 is calibrated at the factory, and the library needs these values to convert the raw readings into °C, % and Pa. After that, a `millis()` timer reads the three values every 2 s.

The address depends on the SDO pin of the sensor: GND → `0x76`, 3V3 → `0x77`. The code simply tries both.

## Wokwi Custom Chip

Wokwi has no built-in BME280, so `chips/bme280.chip.c` simulates one. It is written in C and compiled to WebAssembly (`chips/dist/bme280.chip.wasm`, referenced in `wokwi.toml`). Click on the chip during the simulation to change temperature, pressure, humidity and the I²C address.

To rebuild the chip after changing the C code, run `just build` inside `chips/` (requires podman).

## Exercises

1. Print the temperature in °F as well (`°F = °C × 1.8 + 32`).
2. Show the three values on the OLED from [I²C OLED Display](../esp32-i2c-oled). Both devices share the same I²C bus.
3. Calculate the altitude with `bme.readAltitude(1013.25)`. What does the parameter mean?

## Further Reading

- [Adafruit BME280 library](https://github.com/adafruit/Adafruit_BME280_Library)
- [Bosch BME280 datasheet](https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/)
- [Wokwi custom chips](https://docs.wokwi.com/chips-api/getting-started)
