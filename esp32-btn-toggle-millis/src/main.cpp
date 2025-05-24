#include <Arduino.h>

constexpr uint8_t led = 18;
constexpr uint8_t btn = 34;
constexpr uint8_t buttonDebounceDelay = 50;

// global variables
bool ledState = LOW;
bool lastButtonState = HIGH;
bool lastStableButtonState = HIGH;
uint64_t lastChangeTime = 0;

void setup()
{
  // define led pin as output
  pinMode(led, OUTPUT);
  // define btn pin as input
  pinMode(btn, INPUT);
}

void loop()
{
  // read current button state
  bool currentButtonState = digitalRead(btn);

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
      ledState = !ledState;        // toggle led state
      digitalWrite(led, ledState); // apply led state
    }

    // update stable button state
    lastStableButtonState = currentButtonState;
  }

  // always update the last button state
  lastButtonState = currentButtonState;
}
