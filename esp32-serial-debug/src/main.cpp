#include <Arduino.h>

constexpr uint8_t ledPin = 18;
constexpr uint8_t btnPin = 34;

void setup()
{
  // define gpio pins
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT);

  // Start serial communication with baudrate of 9600
  Serial.begin(9600);

  // Send a status message
  Serial.println("--- Setup finished ---");
}

void loop()
{
  if (digitalRead(btnPin) == LOW) // button pressed (LOW)
  {
    Serial.println("Button pressed -> LED ON!");
    digitalWrite(ledPin, HIGH);
  }
  else // button not pressed (HIGH)
  {
    Serial.println("Button released -> LED OFF!");
    digitalWrite(ledPin, LOW);
  }

  // Add a small delay to make serial output readable
  // Without this, the Serial Monitor might get flooded too quickly.
  delay(250);
}
