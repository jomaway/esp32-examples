#include <Arduino.h>

constexpr uint8_t btnPin = 1;       // D0 = GPIO1
constexpr uint8_t ledRedPin = 2;    // D1 = GPIO2
constexpr uint8_t ledYellowPin = 3; // D2 = GPIO3
constexpr uint8_t ledGreenPin = 4;  // D3 = GPIO4

// Traffic light timing constants
constexpr uint32_t blinkFrequency = 500;

constexpr uint32_t redDuration = 5000;
constexpr uint32_t greenDuration = 5000;
constexpr uint32_t yellowDuration = 2000;

constexpr uint32_t buttonDebounceDelay = 50;

enum TrafficLightState
{
  GREEN,
  RED,
  YELLOW,
  REDYELLOW,
};

TrafficLightState traffic_light_state = RED;
unsigned long lastStateChangeTime = 0;
unsigned long lastBlinkTime = 0;
unsigned long lastButtonChangeTime = 0;

bool yellowLedState = false;
bool nightModeActive = false;

bool lastButtonState = HIGH;
bool lastStableButtonState = HIGH;

void update_lights(TrafficLightState new_state)
{
  switch (new_state)
  {
  case GREEN:
    digitalWrite(ledGreenPin, HIGH);
    digitalWrite(ledRedPin, LOW);
    digitalWrite(ledYellowPin, LOW);
    break;
  case YELLOW:
    digitalWrite(ledGreenPin, LOW);
    digitalWrite(ledRedPin, LOW);
    digitalWrite(ledYellowPin, HIGH);
    break;
  case RED:
    digitalWrite(ledGreenPin, LOW);
    digitalWrite(ledRedPin, HIGH);
    digitalWrite(ledYellowPin, LOW);
    break;
  case REDYELLOW:
    digitalWrite(ledGreenPin, LOW);
    digitalWrite(ledRedPin, HIGH);
    digitalWrite(ledYellowPin, HIGH);
    break;
  }
}

void night_mode()
{
  uint64_t currentMillis = millis();

  if (currentMillis - lastBlinkTime >= blinkFrequency)
  {
    lastBlinkTime = currentMillis;
    yellowLedState = !yellowLedState;
    digitalWrite(ledYellowPin, yellowLedState ? HIGH : LOW);
    digitalWrite(ledRedPin, LOW);
    digitalWrite(ledGreenPin, LOW);
  }
}

void normal_operation()
{
  static uint32_t stateDuration = 0;

  uint64_t currentMillis = millis();

  if (currentMillis - lastStateChangeTime >= stateDuration)
  {
    lastStateChangeTime = currentMillis;

    switch (traffic_light_state)
    {
    case GREEN:
      traffic_light_state = YELLOW;
      stateDuration = yellowDuration;
      break;
    case RED:
      traffic_light_state = REDYELLOW;
      stateDuration = yellowDuration;
      break;
    case YELLOW:
      traffic_light_state = RED;
      stateDuration = redDuration;
      break;
    case REDYELLOW:
      traffic_light_state = GREEN;
      stateDuration = greenDuration;
      break;
    }

    update_lights(traffic_light_state);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(btnPin, INPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledYellowPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
}

void loop()
{
  bool currentButtonState = digitalRead(btnPin);

  if (currentButtonState != lastButtonState)
  {
    lastButtonChangeTime = millis();
    lastButtonState = currentButtonState;
  }

  if (millis() - lastButtonChangeTime >= buttonDebounceDelay)
  {

    if (currentButtonState == LOW && lastStableButtonState == HIGH)
    {
      nightModeActive = !nightModeActive;
      // reset the traffic light state to RED when night mode is activated
      traffic_light_state = RED;

      Serial.println("Button pressed, toggling night mode.");
    }

    // update stable button state
    lastStableButtonState = currentButtonState;
  }

  if (nightModeActive)
  {
    night_mode();
  }
  else
  {
    normal_operation();
  }
}
