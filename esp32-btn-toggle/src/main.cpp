#include <Arduino.h>

constexpr uint8_t led = 18;
constexpr uint8_t btn = 34;

// create some constant variable for our debounce time.
// 30 to 50 ms should be enough
constexpr uint8_t buttonDebounceDelay = 50;

// global Variable for the last button state
// starting with HIGH (not pressed)
bool lastButtonState = HIGH;

// global Variable for the led state.
// starting with led off (LOW)
bool ledState = LOW;

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

  // check for falling edge
  if (currentButtonState == LOW && lastButtonState == HIGH)
  {
    // wait a short time to debounce the button
    delay(buttonDebounceDelay);

    // check if the button is still pressed (LOW)
    if (digitalRead(btn) == LOW)
    {
      ledState = !ledState;        // toggle led state
      digitalWrite(led, ledState); // apply led state
    }
  }
  lastButtonState = currentButtonState;
}
