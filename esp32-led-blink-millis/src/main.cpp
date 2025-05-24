#include <Arduino.h>

constexpr uint8_t led = 18;
constexpr uint64_t blinkFreq = 300;

bool ledState = LOW;         // current led state;
uint64_t lastChangeTime = 0; // last toggle time in ms

void setup()
{
  // define led pin as output
  pinMode(led, OUTPUT);
}

void loop()
{
  // read current millis
  uint64_t currentMillis = millis();

  // check if 300 ms have passed
  if (currentMillis - lastChangeTime >= blinkFreq)
  {
    // toggle led state
    ledState = !ledState;
    digitalWrite(led, ledState);

    // update lastChangeTime with the current value
    lastChangeTime = currentMillis;
  }
}
