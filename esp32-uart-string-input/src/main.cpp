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
    // read all bytes till end of line '\n' into a String
    String inputString = Serial.readStringUntil('\n');
    // remove all leading/trailing whitespaces
    inputString.trim();
    // convert to lowercase for easier comparison
    inputString.toLowerCase();

    if (inputString.equals("led on"))
    {
      ledState = HIGH;
      Serial.println("Led on");
    }
    else if (inputString.equals("led off"))
    {
      ledState = LOW;
      Serial.println("Led off");
    }
    else if (inputString.equals("led toggle"))
    {
      ledState = !ledState;
      Serial.println("Led toggled");
    }
    else if (inputString.equals("led status"))
    {
      // print led state
      Serial.printf("LedState: %s\r\n", ledState ? "on" : "off");
    }
    else if (inputString.equals("help"))
    {
      // display help
      Serial.println("Usage:");
      Serial.println("-------------");
      Serial.println("led on    \t 'Turns the led on' ");
      Serial.println("led off   \t 'Turns the led off'");
      Serial.println("led toggle\t 'Toggle the led state'");
      Serial.println("status    \t 'Prints the current led state'");
      Serial.println("help:     \t 'Display this help'");
    }
    else
    {
      Serial.println("Invalid command");
    }

    digitalWrite(ledPin, ledState);
  }
}
