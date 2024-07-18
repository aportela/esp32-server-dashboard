#include "src/display/ST7789-SPI-240x320-Lovyann/LGFX.hpp"
#include "src/sources/dummy/DummySource.hpp"

#define TFT_CS 5
#define TFT_RST 9
#define TFT_DC 8
#define TFT_SDA 6
#define TFT_SCL 4

LGFX *screen = nullptr;

DummySource *dummySRC = nullptr;

void setup()
{
    screen = new LGFX(TFT_SDA, TFT_SCL, TFT_CS, TFT_DC, TFT_RST);
    screen->init();
    screen->fillScreen(TFT_BLACK);
    screen->drawRect(0, 0, 320, 240, TFT_WHITE);
    screen->initTemperatureMeter(10, 10);
    dummySRC = new DummySource();
}

void loop()
{
    screen->refreshTemperatureMeter(10, 10, dummySRC->getCurrentTemperature());
    delay(50);
}
