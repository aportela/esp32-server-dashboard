#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_MEMORY_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_MEMORY_H

#include "LGFXScreenDashboardResumeEntity.hpp"

class LGFXScreenDashboardResumeEntityUsedMemory : public LGFXScreenDashboardResumeEntity
{
private:
    uint64_t timestamp = 0;
    uint64_t value = 0;
    uint64_t realTotalMemory = 0;
    char oldStrValue[24] = {'\0'};

public:
    LGFXScreenDashboardResumeEntityUsedMemory(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset);
    ~LGFXScreenDashboardResumeEntityUsedMemory();
    bool refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_MEMORY_H
