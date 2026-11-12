#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

constexpr uint8_t screenWidth = 128; // OLED width in pixels
constexpr uint8_t screenHeight = 64; // OLED height in pixels
constexpr uint8_t oledAddress = 0x3C;

constexpr uint32_t updateInterval = 1000; // redraw once per second
constexpr uint8_t maxSeconds = 10;        // the progress bar is full after 10 s

// Create the display object.
// &Wire: the I2C bus it is connected to, -1: no reset pin
Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, -1);

uint32_t lastUpdateTime = 0;
uint8_t seconds = 0;

// Draw the complete screen into the buffer and then send it to the display.
void drawScreen()
{
  // 1. clear the buffer (in RAM – the display does not change yet)
  display.clearDisplay();

  // 2. draw text
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2); // 2x size: 12x16 pixels per character
  display.setCursor(0, 0);
  display.println("Hello!");

  display.setTextSize(1); // normal size: 6x8 pixels per character
  display.setCursor(0, 24);
  display.printf("Seconds: %d", seconds);

  // 3. draw shapes: a progress bar (frame + filled part)
  uint8_t barWidth = map(seconds, 0, maxSeconds, 0, screenWidth - 4);
  display.drawRect(0, 44, screenWidth, 12, SSD1306_WHITE);
  display.fillRect(2, 46, barWidth, 8, SSD1306_WHITE);

  // 4. send the buffer to the display – only now you see the changes
  display.display();
}

void setup()
{
  Serial.begin(115200);

  // initialize the display. SSD1306_SWITCHCAPVCC: generate the display voltage internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, oledAddress))
  {
    Serial.println("SSD1306 not found – check wiring and address!");
    while (true)
    {
      // stop here, nothing else makes sense without a display
    }
  }

  drawScreen();
  lastUpdateTime = millis();
}

void loop()
{
  if (millis() - lastUpdateTime >= updateInterval)
  {
    lastUpdateTime = millis();

    seconds++;
    if (seconds > maxSeconds)
    {
      seconds = 0;
    }

    // only redraw when something changed – a full redraw sends 1 KB over I2C
    drawScreen();
  }
}
