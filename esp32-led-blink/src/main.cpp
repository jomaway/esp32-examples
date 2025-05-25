#include <Arduino.h>

constexpr uint8_t ledPin = 18;

void setup()
{
  // define led pin as output
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  digitalWrite(ledPin, HIGH); // turn led on
  delay(500);                 // wait 500 ms
  digitalWrite(ledPin, LOW);  // turn led off
  delay(500);                 // wait 500 ms
}
