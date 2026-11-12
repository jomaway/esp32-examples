#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

constexpr uint8_t coinButtonPin = 34;
constexpr uint8_t selectButtonPin = 35;
constexpr uint8_t resetButtonPin = 27;
constexpr uint8_t errorLedPin = 19;
constexpr uint8_t readyLedPin = 18;

constexpr uint8_t screenWidth = 128;
constexpr uint8_t screenHeight = 64;
constexpr uint8_t oledAddress = 0x3C;

constexpr uint8_t requiredCoins = 2; // price of an item
constexpr uint8_t maxCoins = 5;      // more coins jam the machine

// timing constants in ms
constexpr uint32_t blinkInterval = 500;
constexpr uint32_t dispenseDuration = 2000;
constexpr uint8_t buttonDebounceDelay = 50;

enum class VendingState
{
  Idle,
  CoinInserted,
  Dispensing,
  OutOfOrder,
};

enum class VendingEvent
{
  CoinDetected,
  ItemSelected,
  Reset,
  TimerExpired,
};

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

Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, -1);

void enterState(VendingState newState);
void handleEvent(VendingEvent event);

VendingState currentState = VendingState::Idle;
uint8_t coinCounter = 0;

uint32_t timerStartTime = 0;
uint32_t timerDuration = 0;
bool timerRunning = false;

// ---------- helpers (same as in "FSM Vending Machine") ----------

bool buttonPressed(Button &button)
{
  bool pressed = false;
  bool currentState = digitalRead(button.pin);

  if (currentState != button.lastState)
  {
    button.lastChangeTime = millis();
  }

  if (millis() - button.lastChangeTime >= buttonDebounceDelay)
  {
    if (currentState == LOW && button.lastStableState == HIGH)
    {
      pressed = true;
    }
    button.lastStableState = currentState;
  }

  button.lastState = currentState;
  return pressed;
}

void startTimer(uint32_t duration)
{
  timerStartTime = millis();
  timerDuration = duration;
  timerRunning = true;
}

void stopTimer()
{
  timerRunning = false;
}

bool timerExpired()
{
  return timerRunning && millis() - timerStartTime >= timerDuration;
}

// ---------- display ----------

// Show a small title and a big text. Called only when something changes,
// never on every loop() run – a full redraw sends 1 KB over I2C.
void showScreen(String title, String text)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(title);
  display.drawLine(0, 10, screenWidth, 10, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 24);
  display.println(text);

  display.display();
}

void showCoins()
{
  showScreen("Coins inserted", String(coinCounter) + " / " + String(requiredCoins));
}

// ---------- setup & loop ----------

void setup()
{
  Serial.begin(115200);
  pinMode(coinButtonPin, INPUT);
  pinMode(selectButtonPin, INPUT);
  pinMode(resetButtonPin, INPUT);
  pinMode(errorLedPin, OUTPUT);
  pinMode(readyLedPin, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, oledAddress))
  {
    Serial.println("SSD1306 not found – check wiring and address!");
  }

  Serial.println("Vending Machine Ready");
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

// ---------- state machine ----------

void addCoin()
{
  coinCounter++;
  Serial.printf("Total coins: %d\r\n", coinCounter);
}

void handleEvent(VendingEvent event)
{
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
      if (coinCounter >= maxCoins)
      {
        enterState(VendingState::OutOfOrder);
      }
      else
      {
        showCoins(); // internal transition: only the coin count changed
      }
    }
    else if (event == VendingEvent::ItemSelected)
    {
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

// Entry actions: LEDs, timer and the screen of each state.
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
    showScreen("Ready", "Insert\ncoin");
    break;

  case VendingState::CoinInserted:
    Serial.println("State: CoinInserted");
    digitalWrite(readyLedPin, LOW);
    digitalWrite(errorLedPin, LOW);
    startTimer(blinkInterval);
    showCoins();
    break;

  case VendingState::Dispensing:
    Serial.println("State: Dispensing");
    digitalWrite(readyLedPin, LOW);
    digitalWrite(errorLedPin, LOW);
    startTimer(dispenseDuration);
    showScreen("Please wait", "Dispensing"); // 10 chars = max. per line at text size 2
    break;

  case VendingState::OutOfOrder:
    Serial.println("State: OutOfOrder");
    digitalWrite(readyLedPin, LOW);
    digitalWrite(errorLedPin, HIGH);
    stopTimer();
    showScreen("Error", "Out of\norder");
    break;
  }
}
