#include <Arduino.h>

constexpr uint8_t ledPin = 18;
constexpr uint8_t btnStartPin = 34;
constexpr uint8_t btnStopPin = 35;

// we need to declare the running variable as volatile because it is modified inside an ISR
volatile bool running = false;

// for demonstration purposes, we will use a busy delay of 2 seconds to simulate some work being done
// this could be some time consuming calculation or similar, but for the sake of this example we will just use a delay
constexpr uint32_t busyDelay = 2000;

// isr for pressing the start button
// ! on esp32 the IRAM_ATTR is needed
void IRAM_ATTR startISR()
{
  // only set a flag, don't do any heavy work inside the ISR!
  running = true;
}

// isr for pressing the stop button
void IRAM_ATTR stopISR()
{
  // only set a flag, don't do any heavy work inside the ISR!
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

  Serial.println("Press the start button to start the work, and the stop button to stop it.");
}

void loop_example_1()
{
  // ! we never check our button state inside the loop
  while (running)
  {
    // pretend to do some important work! (blink an led)
    // this could be some time consuming calculation or similar
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(busyDelay);
    Serial.print('.');
  }

  if (!running)
  {
    digitalWrite(ledPin, LOW);
  }
}

void loop_example_2()
{
  // static means that the variable is only initialized once,
  // and keeps its value between calls to loop()
  static uint32_t lastBlink = 0;

  // ! we never check our button state inside the loop
  while (running)
  {
    // pretend to do some important work! (blink an led)
    // this could be some time consuming calculation or similar

    if (millis() - lastBlink > busyDelay)
    {
      lastBlink = millis();
      digitalWrite(ledPin, !digitalRead(ledPin));
      Serial.print('.');
    }
  }

  if (!running)
  {
    lastBlink = 0;
    digitalWrite(ledPin, LOW);
  }
}

void loop()
{
  loop_example_1();
  // loop_example_2();
}