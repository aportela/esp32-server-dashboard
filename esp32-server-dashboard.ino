
#define TFT_CS 5  // CS puede ser cualquier GPIO
#define TFT_RST 9 // RST puede ser cualquier GPIO
#define TFT_DC 8  // DC puede ser cualquier GPIO
#define TFT_SDA 6 // MOSI, recomendado usar GPIO23
#define TFT_SCL 4 // SCK, recomendado usar GPIO18

#define ADAFRUIT_SPI_SOFTWARE

#ifdef ADAFRUIT_SPI_SOFTWARE

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

Adafruit_ST7789 *screen = nullptr;

#endif

uint8_t values[200];

void setup()
{
#ifdef ADAFRUIT_SPI_SOFTWARE
  screen = new Adafruit_ST7789(TFT_CS, TFT_DC, TFT_SDA, TFT_SCL, TFT_RST);
  screen->init(240, 320);
  screen->setRotation(1);
  screen->fillScreen(ST77XX_BLACK);
#endif
  // init random seed
#ifdef ADAFRUIT_SPI_SOFTWARE
  screen->drawFastVLine(10, 100, 100, ST77XX_WHITE);
  screen->drawFastHLine(10, 200, 200, ST77XX_WHITE);
#endif
  for (int i = 0; i < 200; i++)
  {
    values[i] = 0;
  }
  randomSeed(analogRead(0) ^ (micros() * esp_random()));
  values[199] = random(20, 99);
}

void loop()
{
  // clear old & reload new
  for (int i = 0; i < 200; i++)
  {
    if (values[i] > 0)
    {
#ifdef ADAFRUIT_SPI_SOFTWARE
      // screen->drawPixel(i + 12, 199 - values[i], ST77XX_BLACK);
      screen->drawFastVLine(i + 12, 198 - values[i], values[i], ST77XX_BLACK);
#endif
    }
    if (i < 199)
    {
      values[i] = values[i + 1];
    }
    else
    {
      if (random(5, 10) % 2 == 0)
      {
        if (values[199] < 99)
        {
          values[199]++;
        }
      }
      else if (values[199] > 1)
      {
        values[199]--;
      }
      else
      {
      }
    }
  }
  for (int i = 0; i < 200; i++)
  {
    if (values[i] > 0)
    {
#ifdef ADAFRUIT_SPI_SOFTWARE
      // screen->drawPixel(i + 12, 199 - values[i], ST77XX_RED);
      screen->drawFastVLine(i + 12, 198 - values[i], values[i], ST77XX_RED);
#endif
    }
  }
#ifdef ADAFRUIT_SPI_SOFTWARE
  screen->setCursor(200, 10);
  screen->setTextSize(5);
  screen->setTextColor(ST77XX_BLUE, ST77XX_BLACK);
  screen->setTextWrap(true);
  screen->printf("%02dc", values[199]);
#endif
}
