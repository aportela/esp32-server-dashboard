#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <stdint.h>

#include "src/display/ST7789-SPI-240x320-Lovyann/LGFX.hpp"
#include "src/sources/dummy/DummySource.hpp"

#define TFT_CS 5
#define TFT_RST 9
#define TFT_DC 8
#define TFT_SDA 6
#define TFT_SCL 4

LGFX *screen = nullptr;
lgfx::LGFX_Sprite *graphSprite = nullptr;

uint8_t values[200];

void setup()
{
    screen = new LGFX(TFT_SDA, TFT_SCL, TFT_CS, TFT_DC, TFT_RST);
    screen->init();
    screen->fillScreen(TFT_BLACK);
    screen->drawRect(0, 0, 320, 240, TFT_WHITE);
    graphSprite = new lgfx::LGFX_Sprite(screen);
    graphSprite->createSprite(200, 100);
    graphSprite->fillSprite(TFT_BLACK);
    screen->drawFastVLine(10, 100, 100, TFT_WHITE);
    screen->drawFastHLine(10, 200, 200, TFT_WHITE);
    for (int i = 0; i < 200; i++)
    {
        values[i] = 0;
    }
    // init random seed
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
    values[199] = random(20, 99);
}

void loop()
{
    for (int i = 0; i < 200; i++)
    {
        if (i < 199) // move data array to left
        {
            values[i] = values[i + 1];
        }
        else
        {
            if (random(0, 20) % 2 == 0)
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
        }
    }
    // move graph sprite to left
    graphSprite->scroll(-1, 0);
    int32_t color = 0;
    for (int i = 0; i < 200; i++)
    {
        if (values[i] > 0)
        {
            color = screen->getGradientColor(values[i]);
            graphSprite->drawFastVLine(i, 98 - values[i], values[i], color);
        }
    }

    screen->setCursor(260, 2);
    screen->setTextSize(3);
    screen->setTextColor(color, TFT_BLACK);
    screen->setTextWrap(true);
    screen->printf("%02dc", values[199]);
    graphSprite->pushSprite(12, 100);
}
