#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// SPI data lines use the ESP32 default (VSPI) pins:
// MOSI = GPIO 23, MISO = GPIO 19, SCK = GPIO 18
constexpr uint8_t tftCsPin = 5;   // chip select: LOW = "I'm talking to you"
constexpr uint8_t tftDcPin = 16;  // data/command: tells the display what the bytes mean
constexpr uint8_t tftRstPin = 17; // reset

constexpr uint32_t spiFrequency = 40000000; // 40 MHz
constexpr uint32_t updateInterval = 3000;   // new colour every 3 s

// 320 x 240 pixels, 2 bytes (16 bit colour) per pixel
constexpr uint32_t screenBytes = 320UL * 240UL * 2UL;

// Create the display object. Without pin numbers for MOSI/SCK
// the library uses the hardware SPI on the default pins.
Adafruit_ILI9341 tft(tftCsPin, tftDcPin, tftRstPin);

// background colours in RGB565 format (5 bit red, 6 bit green, 5 bit blue)
constexpr uint8_t colorCount = 4;
const uint16_t colors[colorCount] = {ILI9341_NAVY, ILI9341_DARKGREEN, ILI9341_MAROON, ILI9341_PURPLE};

uint8_t colorIndex = 0;
uint32_t lastUpdateTime = 0;

// Fill the whole screen and measure how long the transfer takes.
void drawSpeedTest()
{
  // micros(): like millis(), but in microseconds – needed for short durations
  uint32_t start = micros();
  tft.fillScreen(colors[colorIndex]);
  uint32_t duration = micros() - start;

  // bytes per microsecond = megabytes per second
  float speed = (float)screenBytes / duration;

  // there is no buffer: every call is sent to the display immediately
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.setCursor(10, 10);
  tft.println("SPI TFT");

  tft.setTextSize(2);
  tft.setCursor(10, 60);
  tft.printf("Full screen: %lu bytes\n", screenBytes);
  tft.setCursor(10, 90);
  tft.printf("Time:  %.1f ms\n", duration / 1000.0);
  tft.setCursor(10, 120);
  tft.printf("Speed: %.2f MB/s\n", speed);

  // a few shapes in custom colours: color565(red, green, blue) with 0-255 each
  tft.fillRect(10, 170, 90, 50, tft.color565(255, 128, 0));
  tft.fillCircle(160, 195, 25, tft.color565(0, 200, 255));
  tft.drawRoundRect(220, 170, 90, 50, 10, ILI9341_YELLOW);

  Serial.printf("fillScreen: %lu us -> %.2f MB/s\r\n", duration, speed);
}

void setup()
{
  Serial.begin(115200);

  // start SPI and initialize the display
  tft.begin(spiFrequency);
  tft.setRotation(1); // landscape: 320 x 240

  drawSpeedTest();
  lastUpdateTime = millis();
}

void loop()
{
  if (millis() - lastUpdateTime >= updateInterval)
  {
    lastUpdateTime = millis();
    colorIndex = (colorIndex + 1) % colorCount;
    drawSpeedTest();
  }
}
