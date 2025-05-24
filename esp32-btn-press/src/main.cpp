#include <Arduino.h>

constexpr uint8_t led = 18;
constexpr uint8_t btn = 34;

void setup()
{
  // define led pin as output
  pinMode(led, OUTPUT);
  // define btn pin as input
  pinMode(btn, INPUT);
}

void loop()
{
  // read the current state of the button and
  // check if button is pressed (LOW)
  if (digitalRead(btn) == LOW)
  {
    digitalWrite(led, HIGH); // turn led on
  }
  else // button is not pressed (HIGH)
  {
    digitalWrite(led, LOW); // turn led off
  }
}
