#include <Arduino.h>

constexpr uint8_t btnPin = 34;
constexpr uint8_t ledRedPin = 19;
constexpr uint8_t ledYellowPin = 18;
constexpr uint8_t ledGreenPin = 5;

// timing constants in ms
constexpr uint32_t redDuration = 5000;
constexpr uint32_t redYellowDuration = 2000;
constexpr uint32_t greenDuration = 5000;
constexpr uint32_t yellowDuration = 2000;
constexpr uint32_t blinkInterval = 500;
constexpr uint8_t buttonDebounceDelay = 50;

// all states the traffic light can be in
enum TrafficLightState
{
  RED,
  REDYELLOW,
  GREEN,
  YELLOW,
  NIGHT,
};

// everything that can happen to the traffic light.
// Events describe WHAT happened – the current state decides what to do.
enum TrafficLightEvent
{
  BUTTON_PRESSED,
  TIMER_EXPIRED,
};

// forward declarations: the functions are defined below loop()
void enterState(TrafficLightState newState);
void handleEvent(TrafficLightEvent event);

TrafficLightState state = RED;

// one timer for all states – started in enterState()
uint32_t timerStartTime = 0;
uint32_t timerDuration = 0;

// night mode: current state of the blinking yellow light
bool yellowState = LOW;

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

// (Re)start the state timer.
void startTimer(uint32_t duration)
{
  timerStartTime = millis();
  timerDuration = duration;
}

// true once the state timer has run out (overflow-safe subtraction)
bool timerExpired()
{
  return millis() - timerStartTime >= timerDuration;
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

  // enter the initial state, so its entry actions (lights, timer) run
  enterState(RED);
}

void loop()
{
  // loop() only generates events – the state machine decides what they mean
  if (buttonPressed())
  {
    handleEvent(BUTTON_PRESSED);
  }

  if (timerExpired())
  {
    handleEvent(TIMER_EXPIRED);
  }
}

// Transitions: decide the next state based on the current state and the event.
void handleEvent(TrafficLightEvent event)
{
  switch (state)
  {
  case RED:
    if (event == TIMER_EXPIRED)
    {
      enterState(REDYELLOW);
    }
    else if (event == BUTTON_PRESSED)
    {
      enterState(NIGHT);
    }
    break;

  case REDYELLOW:
    if (event == TIMER_EXPIRED)
    {
      enterState(GREEN);
    }
    else if (event == BUTTON_PRESSED)
    {
      enterState(NIGHT);
    }
    break;

  case GREEN:
    if (event == TIMER_EXPIRED)
    {
      enterState(YELLOW);
    }
    else if (event == BUTTON_PRESSED)
    {
      enterState(NIGHT);
    }
    break;

  case YELLOW:
    if (event == TIMER_EXPIRED)
    {
      enterState(RED);
    }
    else if (event == BUTTON_PRESSED)
    {
      enterState(NIGHT);
    }
    break;

  case NIGHT:
    if (event == TIMER_EXPIRED)
    {
      // internal transition: stay in NIGHT, only toggle the yellow light
      yellowState = !yellowState;
      setLights(LOW, yellowState, LOW);
      startTimer(blinkInterval);
    }
    else if (event == BUTTON_PRESSED)
    {
      enterState(RED);
    }
    break;
  }
}

// Entry actions: everything that happens once when a state is entered.
void enterState(TrafficLightState newState)
{
  state = newState;

  switch (state)
  {
  case RED:
    Serial.println("State: RED");
    setLights(HIGH, LOW, LOW);
    startTimer(redDuration);
    break;

  case REDYELLOW:
    Serial.println("State: REDYELLOW");
    setLights(HIGH, HIGH, LOW);
    startTimer(redYellowDuration);
    break;

  case GREEN:
    Serial.println("State: GREEN");
    setLights(LOW, LOW, HIGH);
    startTimer(greenDuration);
    break;

  case YELLOW:
    Serial.println("State: YELLOW");
    setLights(LOW, HIGH, LOW);
    startTimer(yellowDuration);
    break;

  case NIGHT:
    Serial.println("State: NIGHT");
    yellowState = HIGH;
    setLights(LOW, yellowState, LOW);
    startTimer(blinkInterval);
    break;
  }
}
