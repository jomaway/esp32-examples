#include <Arduino.h>

constexpr uint8_t ledPin = 18;
constexpr uint8_t potiPin = 35;
constexpr uint16_t threshold = 500;

void setup()
{
  // Start serial communication with baudrate of 9600
  Serial.begin(9600);

  // define gpio pins
  pinMode(ledPin, OUTPUT);
  // the adc pin does not need to be defined.

  // set the ADCs resolution in bits (default: 12)
  // esp32 supports resolution between 9 and 12.
  // 10 means we get values between 0 and 1023 [2^10]
  analogReadResolution(10);

  Serial.println("--- Setup finished ---");
}

void loop()
{
  // read the analog value
  uint16_t potValue = analogRead(potiPin);

  // print value for debugging
  Serial.print("Analog value: ");
  Serial.println(potValue);

  // if value is greater than our threshold
  // turn the led on else turn it off.
  if (potValue >= threshold)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
}
