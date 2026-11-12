#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

constexpr uint8_t i2cSdaPin = 21;
constexpr uint8_t i2cSclPin = 22;

constexpr uint32_t readInterval = 2000; // read the sensor every 2 s

// the sensor object – the library handles all register reads for us
Adafruit_BME280 bme;

uint32_t lastReadTime = 0;

void setup()
{
  Serial.begin(115200);
  Wire.begin(i2cSdaPin, i2cSclPin);

  // The BME280 uses address 0x76 or 0x77, depending on how its SDO pin is wired.
  // Try both – begin() returns false if no sensor answers.
  if (!bme.begin(0x76) && !bme.begin(0x77))
  {
    Serial.println("BME280 not found – check wiring or run the I2C scanner!");
    while (true)
    {
      // stop here, nothing else makes sense without the sensor
    }
  }

  Serial.println("BME280 found.");
}

void loop()
{
  if (millis() - lastReadTime >= readInterval)
  {
    lastReadTime = millis();

    float temperature = bme.readTemperature();     // °C
    float humidity = bme.readHumidity();           // % relative humidity
    float pressure = bme.readPressure() / 100.0F;  // Pa -> hPa

    Serial.printf("Temperature: %.1f C | Humidity: %.1f %% | Pressure: %.1f hPa\r\n",
                  temperature, humidity, pressure);
  }
}
