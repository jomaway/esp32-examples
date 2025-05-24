#include <Arduino.h>

constexpr uint8_t led = 18;

void setup()
{
  // define led pin as output
  pinMode(led, OUTPUT);
}

void loop()
{
  digitalWrite(led, HIGH); // turn led on
  delay(500);              // wait 500 ms
  digitalWrite(led, LOW);  // turn led off
  delay(500);              // wait 500 ms
}
