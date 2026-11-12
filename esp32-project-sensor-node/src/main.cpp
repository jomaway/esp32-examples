#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

constexpr uint8_t btnPin = 34;
constexpr uint8_t neoPixelPin = 4;
constexpr uint8_t i2cSdaPin = 21;
constexpr uint8_t i2cSclPin = 22;

constexpr uint8_t screenWidth = 128;
constexpr uint8_t screenHeight = 64;
constexpr uint8_t oledAddress = 0x3C;

constexpr uint32_t readInterval = 2000; // read the sensor every 2 s
constexpr uint8_t buttonDebounceDelay = 50;

// NeoPixel colour thresholds in °C
constexpr float coldBelow = 20.0;
constexpr float warmAbove = 25.0;

// the pages that can be shown on the display
enum class Page
{
  Temperature,
  Humidity,
  Pressure,
};

Adafruit_BME280 bme;
Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, -1);
Adafruit_NeoPixel neoPixel(1, neoPixelPin, NEO_GRB + NEO_KHZ800);

Page currentPage = Page::Temperature;

// latest measurement
float temperature = 0;
float humidity = 0;
float pressure = 0;
uint32_t lastReadTime = 0;

// button debouncing (same as in "Button Toggle with millis()")
bool lastButtonState = HIGH;
bool lastStableButtonState = HIGH;
uint32_t lastChangeTime = 0;

bool buttonPressed()
{
  bool pressed = false;
  bool currentButtonState = digitalRead(btnPin);

  if (currentButtonState != lastButtonState)
  {
    lastChangeTime = millis();
  }

  if (millis() - lastChangeTime >= buttonDebounceDelay)
  {
    if (currentButtonState == LOW && lastStableButtonState == HIGH)
    {
      pressed = true;
    }
    lastStableButtonState = currentButtonState;
  }

  lastButtonState = currentButtonState;
  return pressed;
}

void setNeoPixelColor(uint8_t red, uint8_t green, uint8_t blue)
{
  neoPixel.setPixelColor(0, neoPixel.Color(red, green, blue));
  neoPixel.show();
}

// Show a small title and a big value.
void showScreen(String title, String value)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(title);
  display.drawLine(0, 10, screenWidth, 10, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 28);
  display.println(value);

  display.display();
}

// Draw the current page with the latest measurement.
void showPage()
{
  switch (currentPage)
  {
  case Page::Temperature:
    showScreen("Temperature  (1/3)", String(temperature, 1) + " C");
    break;
  case Page::Humidity:
    showScreen("Humidity     (2/3)", String(humidity, 1) + " %");
    break;
  case Page::Pressure:
    showScreen("Pressure     (3/3)", String(pressure, 0) + " hPa");
    break;
  }
}

void nextPage()
{
  switch (currentPage)
  {
  case Page::Temperature:
    currentPage = Page::Humidity;
    break;
  case Page::Humidity:
    currentPage = Page::Pressure;
    break;
  case Page::Pressure:
    currentPage = Page::Temperature;
    break;
  }
}

// Status colour: blue = cold, green = comfortable, red = warm.
void updateStatusColor()
{
  if (temperature < coldBelow)
  {
    setNeoPixelColor(0, 0, 255);
  }
  else if (temperature > warmAbove)
  {
    setNeoPixelColor(255, 0, 0);
  }
  else
  {
    setNeoPixelColor(0, 255, 0);
  }
}

void readSensor()
{
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F; // Pa -> hPa

  Serial.printf("Temperature: %.1f C | Humidity: %.1f %% | Pressure: %.1f hPa\r\n",
                temperature, humidity, pressure);
}

void setup()
{
  Serial.begin(115200);
  pinMode(btnPin, INPUT);

  neoPixel.begin();
  setNeoPixelColor(255, 255, 255); // white while starting

  Wire.begin(i2cSdaPin, i2cSclPin);

  if (!display.begin(SSD1306_SWITCHCAPVCC, oledAddress))
  {
    Serial.println("SSD1306 not found – check wiring and address!");
  }

  if (!bme.begin(0x76) && !bme.begin(0x77))
  {
    Serial.println("BME280 not found – check wiring or run the I2C scanner!");
    showScreen("Error", "No sensor");
    setNeoPixelColor(255, 0, 255); // magenta = error
    while (true)
    {
      // stop here, nothing else makes sense without the sensor
    }
  }

  // first measurement right away instead of waiting 2 s
  readSensor();
  updateStatusColor();
  showPage();
  lastReadTime = millis();
}

void loop()
{
  // button: show the next page
  if (buttonPressed())
  {
    nextPage();
    showPage();
  }

  // timer: new measurement
  if (millis() - lastReadTime >= readInterval)
  {
    lastReadTime = millis();
    readSensor();
    updateStatusColor();
    showPage();
  }
}
