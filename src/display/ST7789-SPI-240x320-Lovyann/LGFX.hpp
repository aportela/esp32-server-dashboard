#ifndef ESP32_SERVER_DASHBOARD_LGFX_H
#define ESP32_SERVER_DASHBOARD_LGFX_H

#include <LovyanGFX.hpp>
#include <stdint.h>
#include "LGFXMeter.hpp"
#include "LGFXScreenInfo.hpp"
#include "../../sources/ISource.hpp"
#include "../../sources/SourceData.hpp"

class LGFX : public lgfx::LGFX_Device
{
private:
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::LGFX_Sprite *debugSprite = nullptr;
    LGFXScreenInfo *screenInfo = nullptr;
    ISource *source;

    uint8_t currentScreen = APP_SCREEN_NONE;

public:
    LGFX(uint8_t PIN_SDA, uint8_t PIN_SCL, uint8_t PIN_CS, uint8_t PIN_DC, uint8_t PIN_RST, uint16_t width, uint16_t height, uint8_t rotation);
    ~LGFX();
    void addMeter(LGFXMeter *meter);
    void setSource(ISource *src);
    void refreshDebug(uint16_t xOffset, uint16_t yOffset, int32_t wifiSignalStrength);

    void initScreen(appScreen scr);
    void flipToScreen(appScreen scr);
    void refresh(void);
    void refreshScreenInfo(void);
};

#endif
