#include <Arduino.h>

constexpr uint8_t ledPin = 18;
constexpr uint32_t blinkInterval = 300; // time between two toggles in ms

bool ledState = LOW;         // current led state;
uint32_t lastChangeTime = 0; // last toggle time in ms

void setup()
{
  // define led pin as output
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  // read current millis
  uint32_t currentMillis = millis();

  // check if 300 ms have passed
  if (currentMillis - lastChangeTime >= blinkInterval)
  {
    // toggle led state
    ledState = !ledState;
    digitalWrite(ledPin, ledState);

    // update lastChangeTime with the current value
    lastChangeTime = currentMillis;
  }
}
