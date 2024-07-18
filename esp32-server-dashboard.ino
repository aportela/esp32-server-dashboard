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

DummySource *dummySRC = nullptr;

uint8_t globalTemperature;

void setup()
{
    screen = new LGFX(TFT_SDA, TFT_SCL, TFT_CS, TFT_DC, TFT_RST);
    screen->init();
    screen->fillScreen(TFT_BLACK);
    screen->drawRect(0, 0, 320, 240, TFT_WHITE);
    graphSprite = new lgfx::LGFX_Sprite(screen);
    graphSprite->createSprite(200, 100);
    graphSprite->fillSprite(TFT_BLACK);
    screen->initTemperatureMeter(10, 100);
    dummySRC = new DummySource();
}

void loop()
{
    globalTemperature = dummySRC->getCurrentTemperature();
    // move graph sprite to left
    graphSprite->scroll(-1, 0);
    int32_t color = screen->getGradientColor(globalTemperature);
    graphSprite->drawFastVLine(199, 98 - globalTemperature, globalTemperature, color);

    screen->setCursor(260, 2);
    screen->setTextSize(3);
    screen->setTextColor(color, TFT_BLACK);
    screen->setTextWrap(true);
    screen->printf("%02dc", globalTemperature);
    graphSprite->pushSprite(12, 100);
}
