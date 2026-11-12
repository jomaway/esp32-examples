#include <Arduino.h>
#include <Wire.h>

// The ESP32 uses GPIO 21 (SDA) and GPIO 22 (SCL) as default I2C pins.
constexpr uint8_t i2cSdaPin = 21;
constexpr uint8_t i2cSclPin = 22;

constexpr uint32_t scanInterval = 5000; // scan every 5 s

uint32_t lastScanTime = 0;

// Try to talk to every possible 7-bit address and report who answers.
void scanBus()
{
  uint8_t devicesFound = 0;

  Serial.println("Scanning I2C bus ...");

  // addresses 0x00-0x07 and 0x78-0x7F are reserved
  for (uint8_t address = 0x08; address < 0x78; address++)
  {
    // send only the address (no data) and check if a device acknowledges it
    Wire.beginTransmission(address);
    uint8_t result = Wire.endTransmission();

    if (result == 0) // 0 = ACK received -> a device is listening on this address
    {
      Serial.printf("  Found device at 0x%02X", address);

      // some well-known addresses
      if (address == 0x3C || address == 0x3D)
      {
        Serial.print("  (SSD1306 OLED display?)");
      }
      else if (address == 0x76 || address == 0x77)
      {
        Serial.print("  (BME280 sensor?)");
      }
      Serial.println();

      devicesFound++;
    }
  }

  Serial.printf("Done: %d device(s) found.\r\n\r\n", devicesFound);
}

void setup()
{
  Serial.begin(115200);

  // start the I2C bus as controller on the given pins
  Wire.begin(i2cSdaPin, i2cSclPin);

  scanBus();
  lastScanTime = millis();
}

void loop()
{
  // scan again from time to time – e.g. to see a device you just plugged in
  if (millis() - lastScanTime >= scanInterval)
  {
    scanBus();
    lastScanTime = millis();
  }
}
