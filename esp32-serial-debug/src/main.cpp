#include <Arduino.h>

constexpr uint8_t led = 18;
constexpr uint8_t btn = 34;

void setup()
{
  // define gpio pins
  pinMode(led, OUTPUT);
  pinMode(btn, INPUT);

  // Start serial communication with baudrate of 9600
  Serial.begin(9600);

  // Send a status message
  Serial.println("--- Setup finished ---");
}

void loop()
{
  if (digitalRead(btn) == LOW) // button pressed (LOW)
  {
    Serial.println("Button pressed -> LED ON!");
    digitalWrite(led, HIGH);
  }
  else // button not pressed (HIGH)
  {
    Serial.println("Button released -> LED OFF!");
    digitalWrite(led, LOW);
  }

  // Add a small delay to make serial output readable
  // Without this, the Serial Monitor might get flooded too quickly.
  delay(250);
}
