#include <Arduino.h>

// create an array with all led pins
constexpr uint8_t ledCount = 5;
constexpr uint8_t ledPins[ledCount] = {19, 18, 5, 17, 16};

void setup()
{
  // use a for loop to set all pins as output
  for (int i = 0; i < ledCount; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop()
{
  // turn all leds on
  for (int i = 0; i < ledCount; i++)
  {
    digitalWrite(ledPins[i], HIGH);
    delay(300);
  }

  // turn all leds off
  for (int i = ledCount - 1; i >= 0; i--)
  {
    digitalWrite(ledPins[i], LOW);
    delay(300);
  }
}
