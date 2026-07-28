// note: this example still uses the APIs from ESP_ARDUINO_VERSION_MAJOR == 2
// As platformio ships this version by default.
// See: https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html
#include <Arduino.h>

// Check if the Arduino ESP32 Core version is 3.x or newer
// This is important because the ledc functions used in this code
// are only available in version 3.x and newer.
static_assert(
    ESP_ARDUINO_VERSION_MAJOR >= 3,
    "Please install Arduino ESP32 Core 3.x or newer.");

constexpr uint8_t ledPin = 18;
constexpr uint8_t fadeDelay = 10;
constexpr uint32_t delayTime = 500; // Delay time in milliseconds for fade in/out

constexpr uint16_t pwmFrequence = 5000; // PWM frequency in Hz.
constexpr uint8_t pwmResolution = 8;    // PWM resolution (1-16 bits). 8-bit means 0-255 duty cycle.

void setup()
{
  // Start serial communication with baudrate of 9600
  Serial.begin(9600);

  // config the ledc pwm
  ledcAttach(ledPin, pwmFrequence, pwmResolution);
  Serial.println("--- Setup finished ---");
}

void loop()
{
  // Fade in (0-255)
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++)
  {
    ledcWrite(ledPin, dutyCycle);
    delay(fadeDelay); // Adjust for fade speed
  }

  delay(delayTime); // stay on for the specified time

  // Fade out (255-0)
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--)
  {
    ledcWrite(ledPin, dutyCycle);
    delay(fadeDelay); // Adjust for fade speed
  }

  delay(delayTime); // stay off for the specified time
}
