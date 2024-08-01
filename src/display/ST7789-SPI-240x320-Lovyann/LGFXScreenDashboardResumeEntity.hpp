#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_H

#include <LovyanGFX.hpp>
#include "LGFXMeter.hpp"
#include "../../sources/SourceData.hpp"

#define LGFX_SCR_DRE_FONT &fonts::Font2
#define LGFX_SCR_DRE_FONT_SIZE 1
#define LGFX_SCR_DRE_FONT_COLOR TFT_WHITE
#define LGFX_SCR_DRE_FONT_BG_COLOR TFT_BLACK

#define LGFX_SCR_DRE_TITLE_XOFFSET

class LGFXScreenDashboardResumeEntity
{
private:
protected:
    LGFXMeter *graph = nullptr;
    LovyanGFX *parentDisplay = nullptr;
    SourceData *sourceData = nullptr;
    uint16_t xOffset = 0;
    uint16_t yOffset = 0;
    char strValue[32] = {'\0'};
    bool enabled = false;

public:
    LGFXScreenDashboardResumeEntity(LovyanGFX *display, SourceData *sourceData, uint16_t xOffset, uint16_t yOffset, const char *title, bool enabled = false);
    ~LGFXScreenDashboardResumeEntity();
    virtual bool refresh(bool force = false) = 0;
};

#endif
