#include <Arduino.h>

constexpr uint8_t coinButtonPin = 34;
constexpr uint8_t selectButtonPin = 35;
constexpr uint8_t resetButtonPin = 27;
constexpr uint8_t errorLedPin = 19;
constexpr uint8_t readyLedPin = 18;

constexpr uint8_t requiredCoins = 2; // price of an item
constexpr uint8_t maxCoins = 5;      // more coins jam the machine

// timing constants in ms
constexpr uint32_t blinkInterval = 500;
constexpr uint32_t dispenseDuration = 2000;
constexpr uint8_t buttonDebounceDelay = 50;

// all states of the vending machine (scoped enum: VendingState::Idle)
enum class VendingState
{
  Idle,
  CoinInserted,
  Dispensing,
  OutOfOrder,
};

// everything that can happen to the vending machine
enum class VendingEvent
{
  CoinDetected,
  ItemSelected,
  Reset,
  TimerExpired,
};

// a struct groups all variables that belong to one button
struct Button
{
  uint8_t pin;
  bool lastState;
  bool lastStableState;
  uint32_t lastChangeTime;
};

Button coinButton = {coinButtonPin, HIGH, HIGH, 0};
Button selectButton = {selectButtonPin, HIGH, HIGH, 0};
Button resetButton = {resetButtonPin, HIGH, HIGH, 0};

// forward declarations: the functions are defined below loop()
void enterState(VendingState newState);
void handleEvent(VendingEvent event);

VendingState currentState = VendingState::Idle;
uint8_t coinCounter = 0;

// one timer for all states – started in enterState()
uint32_t timerStartTime = 0;
uint32_t timerDuration = 0;
bool timerRunning = false;

// Returns true exactly once per press of the given button (debounced falling edge).
// The button is passed by reference (&), so the function updates the
// original struct – not a copy. This way one function works for every button.
bool buttonPressed(Button &button)
{
  bool pressed = false;
  bool currentState = digitalRead(button.pin);

  // every change (also bouncing) restarts the debounce timer
  if (currentState != button.lastState)
  {
    button.lastChangeTime = millis();
  }

  // the signal has been stable long enough
  if (millis() - button.lastChangeTime >= buttonDebounceDelay)
  {
    // falling edge of the stable state -> button was pressed
    if (currentState == LOW && button.lastStableState == HIGH)
    {
      pressed = true;
    }
    button.lastStableState = currentState;
  }

  button.lastState = currentState;
  return pressed;
}

// (Re)start the state timer.
void startTimer(uint32_t duration)
{
  timerStartTime = millis();
  timerDuration = duration;
  timerRunning = true;
}

// Not every state needs a timer.
void stopTimer()
{
  timerRunning = false;
}

// true once a running timer has run out (overflow-safe subtraction)
bool timerExpired()
{
  return timerRunning && millis() - timerStartTime >= timerDuration;
}

void setup()
{
  Serial.begin(115200);
  pinMode(coinButtonPin, INPUT);
  pinMode(selectButtonPin, INPUT);
  pinMode(resetButtonPin, INPUT);
  pinMode(errorLedPin, OUTPUT);
  pinMode(readyLedPin, OUTPUT);

  Serial.println("Vending Machine Ready");

  // enter the initial state, so its entry actions run
  enterState(VendingState::Idle);
}

void loop()
{
  // loop() only generates events – the state machine decides what they mean
  if (buttonPressed(coinButton))
  {
    handleEvent(VendingEvent::CoinDetected);
  }

  if (buttonPressed(selectButton))
  {
    handleEvent(VendingEvent::ItemSelected);
  }

  if (buttonPressed(resetButton))
  {
    handleEvent(VendingEvent::Reset);
  }

  if (timerExpired())
  {
    handleEvent(VendingEvent::TimerExpired);
  }
}

void addCoin()
{
  coinCounter++;
  Serial.printf("Total coins: %d\r\n", coinCounter);
}

// Transitions: decide the next state based on the current state and the event.
void handleEvent(VendingEvent event)
{
  Serial.printf("Handling event %d in state %d\r\n", static_cast<int>(event), static_cast<int>(currentState));

  switch (currentState)
  {
  case VendingState::Idle:
    if (event == VendingEvent::CoinDetected)
    {
      addCoin();
      enterState(VendingState::CoinInserted);
    }
    break;

  case VendingState::CoinInserted:
    if (event == VendingEvent::CoinDetected)
    {
      addCoin();
      // guard: too many coins jam the machine
      if (coinCounter >= maxCoins)
      {
        enterState(VendingState::OutOfOrder);
      }
    }
    else if (event == VendingEvent::ItemSelected)
    {
      // guard: only dispense if enough coins were inserted
      if (coinCounter >= requiredCoins)
      {
        enterState(VendingState::Dispensing);
      }
      else
      {
        Serial.println("Not enough coins.");
      }
    }
    else if (event == VendingEvent::TimerExpired)
    {
      // internal transition: stay in CoinInserted, only blink the ready LED
      digitalWrite(readyLedPin, !digitalRead(readyLedPin));
      startTimer(blinkInterval);
    }
    break;

  case VendingState::Dispensing:
    if (event == VendingEvent::TimerExpired)
    {
      Serial.println("Item dispensed.");
      enterState(VendingState::Idle);
    }
    break;

  case VendingState::OutOfOrder:
    if (event == VendingEvent::Reset)
    {
      enterState(VendingState::Idle);
    }
    break;
  }
}

// Entry actions: everything that happens once when a state is entered.
void enterState(VendingState newState)
{
  currentState = newState;

  switch (currentState)
  {
  case VendingState::Idle:
    Serial.println("State: Idle");
    digitalWrite(readyLedPin, HIGH);
    digitalWrite(errorLedPin, LOW);
    coinCounter = 0;
    stopTimer();
    break;

  case VendingState::CoinInserted:
    Serial.println("State: CoinInserted");
    digitalWrite(readyLedPin, LOW);
    digitalWrite(errorLedPin, LOW);
    startTimer(blinkInterval); // blink the ready LED
    break;

  case VendingState::Dispensing:
    Serial.println("State: Dispensing");
    digitalWrite(readyLedPin, LOW);
    digitalWrite(errorLedPin, LOW);
    startTimer(dispenseDuration); // simulate dispensing time
    break;

  case VendingState::OutOfOrder:
    Serial.println("State: OutOfOrder");
    digitalWrite(readyLedPin, LOW);
    digitalWrite(errorLedPin, HIGH);
    stopTimer();
    break;
  }
}
