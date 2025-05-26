#include <Arduino.h>

constexpr uint8_t ledCount = 5;
constexpr uint8_t ledPins[ledCount] = {19, 18, 5, 17, 16};
constexpr uint8_t encPinA = 34;
constexpr uint8_t encPinB = 35;

constexpr int8_t NONE = 0;
constexpr int8_t CLOCKWISE = 1;
constexpr int8_t COUNTERCLOCKWISE = -1;

// Lookup table for state transitions (4x4 matrix, flattened to 16 elements)
// Index: (prev_A << 3) | (prev_B << 2) | (curr_A << 1) | curr_B
// Value: +1 for CW, -1 for CCW, 0 for no change/invalid transition
constexpr int8_t ENCODER_LOOKUP_TABLE[16] = {
    // Previous states: 00, 01, 10, 11
    NONE, CLOCKWISE, COUNTERCLOCKWISE, NONE, // Current state is 00
    COUNTERCLOCKWISE, NONE, NONE, CLOCKWISE, // Current state is 01
    CLOCKWISE, NONE, NONE, COUNTERCLOCKWISE, // Current state is 10
    NONE, COUNTERCLOCKWISE, CLOCKWISE, NONE  // Current state is 11
};

volatile int8_t encDelta = 0;
volatile uint8_t prevEncPinState; // Store previous state (00, 01, 10, 11)

// isr for pressing the start button
// ! on esp32 the IRAM_ATTR is needed
void IRAM_ATTR onEncChange()
{
  // read state of encPinA and encPinB and combine into a 2-bit value (00, 01,10,11)
  uint8_t encPinState = (digitalRead(encPinA) << 1) | digitalRead(encPinB);
  // update Delta based on the rotation.
  encDelta += ENCODER_LOOKUP_TABLE[prevEncPinState << 2 | encPinState];
  // save last state
  prevEncPinState = encPinState;
}

void setup()
{
  Serial.begin(115200);

  for (int i = 0; i < ledCount; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(encPinA, INPUT_PULLUP);
  pinMode(encPinB, INPUT_PULLUP);

  // Read initial state
  prevEncPinState = (digitalRead(encPinA) << 1) | digitalRead(encPinB);

  // call our ISR if a falling edge happens on the defined pins.
  attachInterrupt(encPinA, onEncChange, CHANGE);
  attachInterrupt(encPinB, onEncChange, CHANGE);
}

void loop()
{
  // debug print only if position changed.
  static int encPos = 0;
  static uint8_t currentLed = 0;

  // Atomic read of delta and reset
  noInterrupts();
  int8_t delta = encDelta;
  encDelta = 0;
  interrupts();

  if (delta != 0)
  {
    encPos += delta;
    Serial.printf("Position: %d (delta: %d)\r\n", encPos, delta);

    // Calculate new LED position - handle negative values properly
    int ledIndex = (encPos / 4) % (int)ledCount;
    uint8_t newLed = (ledIndex < 0) ? ledIndex + ledCount : ledIndex;

    // Only update LEDs if position changed
    if (newLed != currentLed)
    {
      digitalWrite(ledPins[currentLed], LOW);
      digitalWrite(ledPins[newLed], HIGH);
      currentLed = newLed;
    }
  }
}
