#include <Arduino.h>

// Check if the Arduino ESP32 Core version is 3.x or newer
// This is important because the timer functions used in this code
// are only available in version 3.x and newer.
static_assert(
    ESP_ARDUINO_VERSION_MAJOR >= 3,
    "Please install Arduino ESP32 Core 3.x or newer.");

constexpr uint8_t ledPin = 19;
constexpr uint32_t timerFrequency = 1000000; // 1 MHz

// global variable to keep track of the LED state
bool ledState = LOW;
// Create a hardware timer object
// Don't initialize the timer here, as this can leed to issues with the ESP32's timer system.
// Instead, initialize it in the setup() function.
hw_timer_t *myTimer = nullptr;

// Interrupt Service Routine (ISR) for the timer
// The IRAM_ATTR attribute is used to place the function in the internal RAM of the ESP32,
// which allows it to be executed faster and avoids potential issues with flash memory access during interrupts.
void IRAM_ATTR blink()
{
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Initialize the hardware timer with the specified frequency
  myTimer = timerBegin(timerFrequency);

  if (myTimer != nullptr)
  {
    // Attach the blink ISR function to the timer interrupt
    timerAttachInterrupt(myTimer, &blink);
    // Timer alarm every 500_000 ticks => at 1 MHz = 500 ms = 0.5 s
    timerAlarm(myTimer, 500000, true, 0);
  }
  else
  {
    Serial.println("Error enable timers");
  }
}

void loop()
{
}
