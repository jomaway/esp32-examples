// note: this example still uses the APIs from ESP_ARDUINO_VERSION_MAJOR == 2
// As platformio ships this version by default.
// See: https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html
#include <Arduino.h>

constexpr uint8_t servoPin = 18;
constexpr uint8_t potiPin = 35;

constexpr uint16_t pwmFrequence = 50; // 50 Hz -> period of 20 ms
constexpr uint8_t pwmResolution = 12; // 0 - 4095
constexpr uint8_t pwmChannel = 0;

void setup()
{
  // config the pwm
  ledcSetup(pwmChannel, pwmFrequence, pwmResolution);
  ledcAttachPin(servoPin, pwmChannel);
}

void loop()
{
  // read the analog value
  uint16_t potValue = analogRead(potiPin);

  // map the analog value to the required duty cycle.
  // servo need 2.5% for 0° and 12.5% for 180°
  uint64_t dutyCycle = map(potValue, 0, 4095, 102, 512);

  ledcWrite(pwmChannel, dutyCycle);
}
