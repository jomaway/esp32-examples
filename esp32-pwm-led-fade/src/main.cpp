// note: this example still uses the APIs from ESP_ARDUINO_VERSION_MAJOR == 2
// As platformio ships this version by default.
// See: https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html
#include <Arduino.h>

constexpr uint8_t ledPin = 18;
constexpr uint8_t fadeDelay = 10;

constexpr uint16_t pwmFrequence = 5000; // PWM frequency in Hz.
constexpr uint8_t pwmResolution = 8;    // PWM resolution (1-16 bits). 8-bit means 0-255 duty cycle.
constexpr uint8_t pwmChannel = 0;

void setup()
{
  // Start serial communication with baudrate of 9600
  Serial.begin(9600);

  // config the ledc pwm
  ledcSetup(pwmChannel, pwmFrequence, pwmResolution);
  ledcAttachPin(ledPin, pwmChannel);

  Serial.println("--- Setup finished ---");
}

void loop()
{
  // Fade in (0-255)
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++)
  {
    ledcWrite(pwmChannel, dutyCycle);
    delay(fadeDelay); // Adjust for fade speed
  }

  delay(500); // stay on for 500 ms

  // Fade out (255-0)
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--)
  {
    ledcWrite(pwmChannel, dutyCycle);
    delay(fadeDelay); // Adjust for fade speed
  }

  delay(500); // stay off for 1 s
}
