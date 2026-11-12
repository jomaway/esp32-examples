#include <Arduino.h>

constexpr uint8_t ledPin = 18;
constexpr uint8_t btnPin = 34;
constexpr uint8_t sampleIntervalMs = 5;

// global variables
uint8_t buttonHistory = 0xFF; // initialize to all HIGH (not pressed)
bool ledState = LOW;
bool lastStableButtonState = HIGH;
uint32_t lastSampleTime = 0;

void setup()
{
  // define led pin as output
  pinMode(ledPin, OUTPUT);
  // define btn pin as input
  pinMode(btnPin, INPUT);

  Serial.begin(9600);
}

void loop()
{
  uint32_t currentTime = millis();

  if (currentTime - lastSampleTime >= sampleIntervalMs)
  {
    // update the last sample time
    lastSampleTime = currentTime;

    // read current button state.  Active-low: LOW (0) means pressed, HIGH (1) means not pressed
    bool currentButtonState = digitalRead(btnPin);

    // update button history
    // Shift in the raw reading.
    buttonHistory = (buttonHistory << 1) | currentButtonState;

    if ((buttonHistory & 0x07) == 0x00) // last 3 bits are LOW (pressed)
    {
      // check for falling edge
      if (lastStableButtonState == HIGH) // last stable state was not pressed
      {
        ledState = !ledState;           // toggle led state
        digitalWrite(ledPin, ledState); // apply led state
        Serial.println("Button pressed, LED toggled");
      }
      lastStableButtonState = LOW; // update stable button state to pressed
    }
    else if (buttonHistory == 0xFF) // all bits are HIGH (not pressed)
    {
      if (lastStableButtonState == LOW) // last stable state was pressed
      {
        Serial.println("Button released");
      }
      lastStableButtonState = HIGH; // update stable button state to not pressed
    }
  }
}
