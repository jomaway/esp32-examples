#include <Arduino.h>

constexpr uint8_t btnPin = 34;
constexpr uint8_t ledRedPin = 19;
constexpr uint8_t ledYellowPin = 18;
constexpr uint8_t ledGreenPin = 5;

// timing constants in ms
constexpr uint32_t redDuration = 5000;
constexpr uint32_t greenDuration = 5000;
constexpr uint32_t yellowDuration = 2000;
constexpr uint32_t blinkInterval = 500;
constexpr uint8_t buttonDebounceDelay = 50;

// normal mode: current phase of the traffic light
// 0 = red, 1 = green, 2 = yellow
uint8_t phase = 0;
uint32_t phaseStartTime = 0;

// night mode: blinking yellow light
bool nightMode = false;
bool yellowState = LOW;
uint32_t lastBlinkTime = 0;

// button debouncing (same as in "Button Toggle with millis()")
bool lastButtonState = HIGH;
bool lastStableButtonState = HIGH;
uint32_t lastChangeTime = 0;

// Returns true exactly once per button press (debounced falling edge).
// Must be called on every loop() run – it never waits.
bool buttonPressed()
{
  bool pressed = false;
  bool currentButtonState = digitalRead(btnPin);

  // every change (also bouncing) restarts the debounce timer
  if (currentButtonState != lastButtonState)
  {
    lastChangeTime = millis();
  }

  // the signal has been stable long enough
  if (millis() - lastChangeTime >= buttonDebounceDelay)
  {
    // falling edge of the stable state -> button was pressed
    if (currentButtonState == LOW && lastStableButtonState == HIGH)
    {
      pressed = true;
    }
    lastStableButtonState = currentButtonState;
  }

  lastButtonState = currentButtonState;
  return pressed;
}

// Switch all three lights at once.
void setLights(bool red, bool yellow, bool green)
{
  digitalWrite(ledRedPin, red);
  digitalWrite(ledYellowPin, yellow);
  digitalWrite(ledGreenPin, green);
}

void setup()
{
  Serial.begin(115200);
  pinMode(btnPin, INPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledYellowPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);

  // start in normal mode with red
  setLights(HIGH, LOW, LOW);
  phaseStartTime = millis();
}

void loop()
{
  // 1. read the button and switch between normal and night mode
  if (buttonPressed())
  {
    nightMode = !nightMode;

    if (nightMode)
    {
      Serial.println("Night mode on");
      // start blinking immediately with yellow on
      yellowState = HIGH;
      setLights(LOW, yellowState, LOW);
      lastBlinkTime = millis();
    }
    else
    {
      Serial.println("Night mode off");
      // restart normal mode with a full red phase
      phase = 0;
      setLights(HIGH, LOW, LOW);
      phaseStartTime = millis();
    }
  }

  uint32_t currentTime = millis();

  // 2. update the lights depending on the mode
  if (nightMode)
  {
    // blink yellow
    if (currentTime - lastBlinkTime >= blinkInterval)
    {
      yellowState = !yellowState;
      setLights(LOW, yellowState, LOW);
      lastBlinkTime = currentTime;
    }
  }
  else
  {
    // normal mode: switch to the next phase when the current one is over
    if (phase == 0 && currentTime - phaseStartTime >= redDuration)
    {
      phase = 1; // red -> green
      setLights(LOW, LOW, HIGH);
      phaseStartTime = currentTime;
    }
    else if (phase == 1 && currentTime - phaseStartTime >= greenDuration)
    {
      phase = 2; // green -> yellow
      setLights(LOW, HIGH, LOW);
      phaseStartTime = currentTime;
    }
    else if (phase == 2 && currentTime - phaseStartTime >= yellowDuration)
    {
      phase = 0; // yellow -> red
      setLights(HIGH, LOW, LOW);
      phaseStartTime = currentTime;
    }
  }
}
