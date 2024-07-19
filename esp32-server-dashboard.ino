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
    // screen->drawRect(0, 0, 320, 240, TFT_WHITE); // this is for screen bounds debugging purposes only
    screen->initCPULoadMeter(0, 0);
    screen->initMemoryMeter(0, 42);
    screen->initCPUTemperatureMeter(0, 84);
    screen->initNetworkDownloadBandwithMeter(0, 126);
    screen->initNetworkUploadBandwithMeter(0, 168);
    dummySRC = new DummySource();
}

void loop()
{
    screen->refreshCPULoadMeter(0, 0, dummySRC->getCurrentCPULoad());
    screen->refreshMemoryMeter(0, 42, dummySRC->getUsedMemory());
    screen->refreshCPUTemperatureMeter(0, 84, dummySRC->getCurrentCPUTemperature());
    screen->refreshNetworkDownloadBandwithMeter(0, 126, dummySRC->getUsedNetworkDownloadBandwith());
    screen->refreshNetworkUploadBandwithMeter(0, 168, dummySRC->getUsedNetworkUploadBandwith());
    screen->refreshDebug(0, 220);
    // delay(50);
}
