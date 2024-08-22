#ifndef ESP32_SERVER_DASHBOARD_LGFX_H
#define ESP32_SERVER_DASHBOARD_LGFX_H

#include <LovyanGFX.hpp>
#include <stdint.h>
#include "LGFXScreen.hpp"
#include "../ScreenType.hpp"
#include "../../sources/SourceData.hpp"

class LGFX : public lgfx::LGFX_Device
{
private:
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;

    SourceData *sourceData;

    ScreenType currentScreenType = ST_NONE;

    LGFXScreen *currentScreen = nullptr;

    void deleteCurrentScreen(void);

public:
    LGFX(uint8_t PIN_SDA, uint8_t PIN_SCL, uint8_t PIN_CS, uint8_t PIN_DC, uint8_t PIN_RST, uint16_t width, uint16_t height, uint8_t rotation);
    ~LGFX();
    void setSourceData(SourceData *src);
    void initScreen(ScreenType scr);
    ScreenType getCurrentScreenType(void);
    bool flipToScreen(ScreenType scr);
    bool toggleScreen(void);
    bool refresh(void);
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_H
