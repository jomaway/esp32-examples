#include <Arduino.h>

constexpr uint8_t ledPin = 18;
constexpr uint8_t btnPin = 34;
constexpr uint8_t buttonDebounceDelay = 50;

// global variables
bool ledState = LOW;
bool lastButtonState = HIGH;
bool lastStableButtonState = HIGH;
uint64_t lastChangeTime = 0;

void setup()
{
  // define led pin as output
  pinMode(ledPin, OUTPUT);
  // define btn pin as input
  pinMode(btnPin, INPUT);
}

void loop()
{
  // read current button state
  bool currentButtonState = digitalRead(btnPin);

  // check for falling and rising edge
  if (currentButtonState != lastButtonState)
  {
    lastChangeTime = millis();
  }

  if (millis() - lastChangeTime >= buttonDebounceDelay)
  {
    // check for falling edge
    if (currentButtonState == LOW && lastStableButtonState == HIGH)
    {
      ledState = !ledState;           // toggle led state
      digitalWrite(ledPin, ledState); // apply led state
    }

    // update stable button state
    lastStableButtonState = currentButtonState;
  }

  // always update the last button state
  lastButtonState = currentButtonState;
}
