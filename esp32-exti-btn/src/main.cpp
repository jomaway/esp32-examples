#include <Arduino.h>

constexpr uint8_t ledPin = 18;
constexpr uint8_t btnStartPin = 34;
constexpr uint8_t btnStopPin = 35;

volatile bool running = false;

// isr for pressing the start button
// ! on esp32 the IRAM_ATTR is needed
void IRAM_ATTR startISR()
{
  running = true;
}

// isr for pressing the stop button
void IRAM_ATTR stopISR()
{
  running = false;
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(btnStartPin, INPUT);
  pinMode(btnStopPin, INPUT);

  // call our ISR if a falling edge happens on the defined pins.
  attachInterrupt(btnStartPin, startISR, FALLING);
  attachInterrupt(btnStopPin, stopISR, FALLING);
}

void loop()
{
  // ! we never check our button state inside the loop

  while (running)
  {
    // pretend to do some important work! (blink an led)
    // this could be some time consuming calculation or similar
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(300);
    Serial.print('.');
  }

  if (!running)
  {
    digitalWrite(ledPin, LOW);
  }
}
