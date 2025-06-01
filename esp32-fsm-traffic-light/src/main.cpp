#include <Arduino.h>

constexpr uint8_t redLedPin = 19;
constexpr uint8_t yellowLedPin = 18;
constexpr uint8_t greenLedPin = 5;
constexpr uint8_t buttonPin = 34;

enum TrafficLightState
{
  OFF,
  RED,
  REDYELLOW,
  YELLOW,
  GREEN,
};

enum TrafficLightEvent
{
  ONOFF,
  NEXT,
};

void handleEvent(TrafficLightEvent event);

TrafficLightState state = TrafficLightState::OFF;
uint64_t nextTransitionTime = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop()
{
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(buttonPin);
  if (currentButtonState == LOW && lastButtonState == HIGH)
  {
    delay(60);                         // Debounce delay
    if (digitalRead(buttonPin) == LOW) // Check again after debounce
    {
      Serial.println("Button pressed");
      handleEvent(ONOFF);
    }
  }
  lastButtonState = currentButtonState;

  if (state == OFF)
  {
    if (millis() % 1000 < 500) // Blink every 500 ms
    {
      digitalWrite(redLedPin, HIGH);
      digitalWrite(yellowLedPin, HIGH);
      digitalWrite(greenLedPin, HIGH);
    }
    else
    {
      digitalWrite(redLedPin, LOW);
      digitalWrite(yellowLedPin, LOW);
      digitalWrite(greenLedPin, LOW);
    }
  }
  else
  {
    if (millis() >= nextTransitionTime)
    {
      handleEvent(NEXT);
    }
  }
}

void enterState(const TrafficLightState newState)
{
  state = newState;

  switch (state)
  {
  case OFF:
    Serial.println("Entering OFF state");
    digitalWrite(redLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    break;
  case RED:
    Serial.println("Entering RED state");
    digitalWrite(redLedPin, HIGH);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    nextTransitionTime = millis() + 5000;
    break;
  case REDYELLOW:
    Serial.println("Entering REDYELLOW state");
    digitalWrite(redLedPin, HIGH);
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    nextTransitionTime = millis() + 2000;
    break;
  case YELLOW:
    Serial.println("Entering YELLOW state");
    digitalWrite(redLedPin, LOW);
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    nextTransitionTime = millis() + 2000;
    break;
  case GREEN:
    Serial.println("Entering GREEN state");
    digitalWrite(redLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    nextTransitionTime = millis() + 5000;
    break;
  }
}

void handleEvent(const TrafficLightEvent event)
{
  Serial.printf("Handling event: %d in state: %d\r\n", event, state);
  switch (state)
  {
  case OFF:
    if (event == ONOFF)
    {
      enterState(RED);
    }
    break;
  case RED:
    if (event == ONOFF)
    {
      enterState(OFF);
    }
    else if (event == NEXT)
    {
      enterState(REDYELLOW);
    }
    break;
  case REDYELLOW:
    if (event == ONOFF)
    {
      enterState(OFF);
    }
    else if (event == NEXT)
    {
      enterState(GREEN);
    }
    break;
  case GREEN:
    if (event == ONOFF)
    {
      enterState(OFF);
    }
    else if (event == NEXT)
    {
      enterState(YELLOW);
    }
    break;
  case YELLOW:
    if (event == ONOFF)
    {
      enterState(OFF);
    }
    else if (event == NEXT)
    {
      enterState(RED);
    }
    break;
  }
}