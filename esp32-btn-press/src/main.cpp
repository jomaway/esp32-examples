#include <Arduino.h>

constexpr uint8_t ledPin = 18;
constexpr uint8_t btnPin = 34;

void setup()
{
  // define led pin as output
  pinMode(ledPin, OUTPUT);
  // define btn pin as input
  pinMode(btnPin, INPUT);
}

void loop()
{
  // read the current state of the button and
  // check if button is pressed (LOW)
  if (digitalRead(btnPin) == LOW)
  {
    digitalWrite(ledPin, HIGH); // turn led on
  }
  else // button is not pressed (HIGH)
  {
    digitalWrite(ledPin, LOW); // turn led off
  }
}
