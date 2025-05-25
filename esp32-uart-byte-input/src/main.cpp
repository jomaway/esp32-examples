#include <Arduino.h>

constexpr uint8_t ledPin = 18;

bool ledState = LOW;

void setup()
{
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  Serial.println("--- Setup finished ---");
}

void loop()
{
  // check if some data is ready to read
  if (Serial.available())
  {
    // read one byte into our byte variable
    uint8_t receivedByte = Serial.read();

    // for debugging uncomment next line
    // Serial.println(receivedByte, HEX); // send byte back to pc.

    switch (receivedByte)
    {
    case 'o':          // ASCI 0x6F
      ledState = HIGH; // led on
      break;
    case 'r':         // ASCI 0x72
      ledState = LOW; // led off
      break;
    case 0x74:              // ASCI 't'
      ledState = !ledState; // toggle led
      break;
    default:
      Serial.println("Invalid byte received");
      break;
    }

    digitalWrite(ledPin, ledState);
  }
}
