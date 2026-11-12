# ESP32 I²C Scanner

Find all devices on the I²C bus. An OLED display and a BME280 sensor share the same two wires, and the scanner tells you which addresses answer.

> **Learning path:** 7. Buses: I²C and SPI · [← Previous: UART String Input](../esp32-uart-string-input) · [Next: I²C OLED Display →](../esp32-i2c-oled)

## New Concepts

- The **I²C bus**: two wires (SDA = data, SCL = clock) shared by many devices
- **Controller and targets**: the ESP32 starts every transfer, and the devices only answer
- **7-bit addresses**: every device has its own address (OLED `0x3C`, BME280 `0x76`)
- **ACK / NACK**: a device confirms ("acknowledges") its address. No answer means nobody is there
- **Pull-up resistors**: I²C lines are only pulled *low* by the devices. The resistors pull them back up
- The `Wire` library: `Wire.begin(sda, scl)`, `beginTransmission()`, `endTransmission()`
- Hexadecimal output with `printf("%02X")`
- Watching the protocol with a **logic analyzer**

## Wiring

All I²C devices are connected **in parallel** to the same two lines.

| Part              | ESP32 pin | Notes                                  |
| ----------------- | --------- | -------------------------------------- |
| SDA (all devices) | GPIO 21   | 4.7 kΩ pull-up to 3V3                  |
| SCL (all devices) | GPIO 22   | 4.7 kΩ pull-up to 3V3                  |
| OLED VCC / GND    | 3V3 / GND | SSD1306, address `0x3C`                |
| BME280 VCC / GND  | 3V3 / GND | address `0x76` (or `0x77`)             |
| Logic analyzer    | D0 / D1   | D0 = SCL, D1 = SDA (Wokwi only)        |

Serial monitor: 115200 baud.

> **Note:** Many breakout boards already have pull-up resistors on board. With several boards on one bus the pull-ups are in parallel, which is usually still fine.

## How it works

For every possible address the ESP32 sends a *start condition* and the address. Then it checks whether a device pulls SDA low to acknowledge:

```cpp
Wire.beginTransmission(address);
uint8_t result = Wire.endTransmission(); // 0 = ACK -> device found
```

Addresses `0x00`–`0x07` and `0x78`–`0x7F` are reserved, so the scan runs from `0x08` to `0x77`. The scan repeats every 5 s with `millis()`, so you can watch a device appear when you connect it.

Expected output:

```
Scanning I2C bus ...
  Found device at 0x3C  (SSD1306 OLED display?)
  Found device at 0x76  (BME280 sensor?)
Done: 2 device(s) found.
```

## Logic Analyzer (Wokwi)

The simulation records SCL and SDA into `i2c-scanner-capture.vcd`. Open it in [PulseView](https://sigrok.org/wiki/PulseView) and add the **I²C** protocol decoder (SCL = D0, SDA = D1). You will see a short transfer for every address: *start – address – write bit – NACK – stop*, except for the two addresses that answer with an **ACK**.

## Exercises

1. Change the address of the BME280 in Wokwi (chip settings: `0x76` ↔ `0x77`) and scan again.
2. Remove one pull-up resistor in the simulation. What happens? Why are pull-ups needed?
3. In the logic analyzer capture, find the address `0x3C`. How many clock pulses does one address byte take, including the ACK bit?

## Further Reading

- [SparkFun: I²C tutorial](https://learn.sparkfun.com/tutorials/i2c)
- [Arduino reference: `Wire`](https://docs.arduino.cc/language-reference/en/functions/communication/wire/)
- [Wokwi logic analyzer](https://docs.wokwi.com/guides/logic-analyzer)
