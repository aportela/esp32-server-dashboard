#ifndef ESP32_SERVER_DASHBOARD_LGFX_H
#define ESP32_SERVER_DASHBOARD_LGFX_H

#include <LovyanGFX.hpp>
#include <stdint.h>
#include "LGFXScreen.hpp"
#include "../ScreenType.hpp"
#include "../../sources/SourceData.hpp"
#include "../DashboardItemType.hpp"

#define MAX_DASHBOARDS 3
#define DASHBOARD_ITEM_COUNT 5

class LGFX : public lgfx::LGFX_Device
{
private:
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;

    SourceData *sourceData;

    SCREEN_TYPE currentScreenType = SCREEN_TYPE_NONE;

    LGFXScreen *currentScreen = nullptr;

    uint8_t dashboardCount = 0;
    uint8_t dashboardsItems[MAX_DASHBOARDS][DASHBOARD_ITEM_COUNT] = {0};
    uint8_t currentDashboardIndex = 0;

    void DeleteCurrentScreen(void);

public:
    LGFX(uint8_t PIN_SDA, uint8_t PIN_SCL, uint8_t PIN_CS, uint8_t PIN_DC, uint8_t PIN_RST, uint16_t width, uint16_t height, uint8_t rotation);
    ~LGFX();
    void SetSourceData(SourceData *src);
    bool setDashboardCount(uint8_t count);
    bool SetDashboardItems(uint8_t index, const DASHBOARD_ITEM_TYPE items[DASHBOARD_ITEM_COUNT]);
    void InitScreen(SCREEN_TYPE scr);
    SCREEN_TYPE GetCurrentScreenType(void);
    bool FlipToScreen(SCREEN_TYPE scr);
    bool ToggleScreen(void);
    bool Refresh(void);
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_H
