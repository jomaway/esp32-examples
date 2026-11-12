#include <Arduino.h>

constexpr uint8_t ledPin = 18;
constexpr uint8_t btnPin = 34;
constexpr uint8_t buttonDebounceDelay = 50;

// global variables
bool ledState = LOW;
bool lastButtonState = HIGH;
bool lastStableButtonState = HIGH;
uint32_t lastChangeTime = 0;

// Returns true exactly once per button press (debounced falling edge).
// Must be called on every loop() run – it never waits.
bool buttonPressed()
{
  bool pressed = false;
  bool currentButtonState = digitalRead(btnPin);

  // every change (also bouncing) restarts the debounce timer
  if (currentButtonState != lastButtonState)
  {
    lastChangeTime = millis();
  }

  // the signal has been stable long enough
  if (millis() - lastChangeTime >= buttonDebounceDelay)
  {
    // falling edge of the stable state -> button was pressed
    if (currentButtonState == LOW && lastStableButtonState == HIGH)
    {
      pressed = true;
    }

    // update stable button state
    lastStableButtonState = currentButtonState;
  }

  // always update the last button state
  lastButtonState = currentButtonState;

  // single exit point: the state above is always updated before returning
  return pressed;
}

void setup()
{
  // define led pin as output
  pinMode(ledPin, OUTPUT);
  // define btn pin as input
  pinMode(btnPin, INPUT);
}

void loop()
{
  if (buttonPressed())
  {
    ledState = !ledState;           // toggle led state
    digitalWrite(ledPin, ledState); // apply led state
  }
}
