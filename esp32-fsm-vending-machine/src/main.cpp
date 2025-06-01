#include <Arduino.h>

constexpr uint8_t coinSlotPin = 34;
constexpr uint8_t selectButtonPin = 35;
constexpr uint8_t resetButtonPin = 27;
constexpr uint8_t errorLedPin = 19;
constexpr uint8_t readyLedPin = 18;

enum class VendingState
{
  Idle,
  CoinInserted,
  Dispensing,
  OutOfOrder
};

enum class VendingEvent
{
  CoinDetected,
  ItemSelected,
  DispenseComplete,
  ErrorOccured,
  Reset,
};

void handleEvent(VendingEvent event);
void enterState(VendingState state);

VendingState currentState;
uint8_t currentCoinCounter = 0;
constexpr uint8_t requiredCoins = 2;

void setup()
{
  Serial.begin(115200);
  pinMode(coinSlotPin, INPUT);
  pinMode(selectButtonPin, INPUT);
  pinMode(resetButtonPin, INPUT);
  pinMode(errorLedPin, OUTPUT);
  pinMode(readyLedPin, OUTPUT);

  Serial.println("Vending Machine Ready");
  enterState(VendingState::Idle);
}

void checkButtonPressed(uint8_t buttonPin, bool &lastButtonState, VendingEvent event)
{
  bool currentButtonState = digitalRead(buttonPin);
  if (currentButtonState == LOW && lastButtonState == HIGH)
  {
    delay(50);                         // debounce delay
    if (digitalRead(buttonPin) == LOW) // confirm button is still pressed
    {
      handleEvent(event);
    }
  }
  lastButtonState = currentButtonState;
}

void loop()
{
  static bool lastCoinButtonState = HIGH; // last state of the coin slot button
  checkButtonPressed(coinSlotPin, lastCoinButtonState, VendingEvent::CoinDetected);

  static bool lastSelectButtonState = HIGH; // last state of the select button
  checkButtonPressed(selectButtonPin, lastSelectButtonState, VendingEvent::ItemSelected);

  static bool lastResetButtonState = HIGH; // last state of the reset button
  checkButtonPressed(resetButtonPin, lastResetButtonState, VendingEvent::Reset);

  if (currentState == VendingState::CoinInserted)
  {
    // blink the ready LED to indicate coins are inserted
    static unsigned long lastBlinkTime = 0;
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= 500) // blink every 500 ms
    {
      digitalWrite(readyLedPin, !digitalRead(readyLedPin)); // toggle LED state
      lastBlinkTime = currentTime;
    }
  }

  // simulate an error condition for demonstration purposes
  if (currentState != VendingState::OutOfOrder && currentCoinCounter >= 5) // if too many coins are inserted
  {
    handleEvent(VendingEvent::ErrorOccured);
  }
}

void enterState(const VendingState newState)
{
  currentState = newState;

  switch (currentState)
  {
  case VendingState::Idle:
    digitalWrite(readyLedPin, HIGH);
    digitalWrite(errorLedPin, LOW);
    currentCoinCounter = 0;
    Serial.println("State: Idle");
    break;
  case VendingState::CoinInserted:
    digitalWrite(readyLedPin, LOW);
    digitalWrite(errorLedPin, LOW);
    Serial.println("State: CoinInserted ");
    /* code */
    break;
  case VendingState::Dispensing:
    digitalWrite(readyLedPin, LOW);
    digitalWrite(errorLedPin, LOW);
    Serial.println("State: Dispensing");
    delay(2000); // Simulate dispensing time
    handleEvent(VendingEvent::DispenseComplete);
    break;
  case VendingState::OutOfOrder:
    digitalWrite(readyLedPin, LOW);
    digitalWrite(errorLedPin, HIGH);
    Serial.println("State: OutOfOrder");
    break;
  }
}

void incrementCoins()
{
  currentCoinCounter++;
  Serial.printf("Total coins: %d\r\n", currentCoinCounter);
}

void handleEvent(const VendingEvent event)
{
  Serial.printf("Handling event: %d in state: %d\r\n", static_cast<int>(event), static_cast<int>(currentState));
  switch (currentState)
  {
  case VendingState::Idle:
    if (event == VendingEvent::CoinDetected)
    {
      enterState(VendingState::CoinInserted);
      incrementCoins();
    }
    break;
  case VendingState::CoinInserted:
    if (event == VendingEvent::CoinDetected)
    {
      incrementCoins();
    }
    else if (event == VendingEvent::ItemSelected)
    {
      if (currentCoinCounter >= requiredCoins)
      {
        enterState(VendingState::Dispensing);
      }
      else
      {
        Serial.println("Not enough coins.");
      }
    }
    else if (event == VendingEvent::ErrorOccured)
    {
      enterState(VendingState::OutOfOrder);
    }
    /* code */
    break;
  case VendingState::Dispensing:
    if (event == VendingEvent::DispenseComplete)
    {
      enterState(VendingState::Idle);
    }
    else if (event == VendingEvent::ErrorOccured)
    {
      enterState(VendingState::OutOfOrder);
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