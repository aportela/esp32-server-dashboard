#define DISPLAY_DRIVER_LOVYANN_ST7789 // at this time, only LovyAnn ST7789 driver support

#ifdef DISPLAY_DRIVER_LOVYANN_ST7789

#include "src/display/ST7789-SPI-240x320-Lovyann/LGFX.hpp"
#include "src/display/ST7789-SPI-240x320-Lovyann/LGFXMeter.hpp"

#define PIN_CS 5
#define PIN_RST 9
#define PIN_DC 8
#define PIN_SDA 6
#define PIN_SCL 4

// these are my custom/valid values for a ST7789 240x320 screen
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define SCREEN_ROTATION 3

LGFX *screen = nullptr;
LGFXMeter *cpuLoadMeter = nullptr;

#else
#error NO_DISPLAY_DRIVER
#endif

#include "src/sources/Source.hpp"
#include "src/sources/dummy/DummySource.hpp"

Source *dummySRC = nullptr;

void setup()
{
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    dummySRC = new DummySource();
    screen = new LGFX(PIN_SDA, PIN_SCL, PIN_CS, PIN_DC, PIN_RST, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ROTATION);
    screen->init();
    screen->fillScreen(TFT_BLACK);
    // screen->drawRect(0, 0, 320, 240, TFT_WHITE); // this is for screen bounds debugging purposes only
    screen->initCPULoadMeter(0, 0);
    // screen->initMemoryMeter(0, 42);
    // screen->initCPUTemperatureMeter(0, 84);
    // screen->initNetworkDownloadBandwithMeter(0, 126);
    // screen->initNetworkUploadBandwithMeter(0, 168);
    screen->setSource(dummySRC);
    cpuLoadMeter = new LGFXMeter(screen, 200, 30, 0, 42, TFT_BLACK, "CPU LOAD");
#endif
}

void loop()
{
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    dummySRC->refresh();
    // screen->refreshCPULoadMeter(0, 0);
    //   screen->refreshMemoryMeter(0, 42);
    //   screen->refreshCPUTemperatureMeter(0, 84);
    //   screen->refreshNetworkDownloadBandwithMeter(0, 126);
    //   screen->refreshNetworkUploadBandwithMeter(0, 168);
    cpuLoadMeter->refresh(dummySRC->getCurrentCPULoad());
    screen->refreshDebug(0, 210);
#else
    delay(50);
#endif
}
