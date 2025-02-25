#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_MEMORY_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_MEMORY_H

#include "LGFXScreenDashboardResumeEntity.hpp"
#include "../DashboardItemType.hpp"

class LGFXScreenDashboardResumeEntityMemory : public LGFXScreenDashboardResumeEntity
{
private:
    uint64_t timestamp = 0;
    uint64_t value = 0;
    uint64_t totalValue = 0;
    char oldStrValue[24] = {'\0'};
    DASHBOARD_ITEM_TYPE memoryUsageType = DASHBOARD_ITEM_TYPE_MEM_USED;

public:
    LGFXScreenDashboardResumeEntityMemory(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, DASHBOARD_ITEM_TYPE memoryUsageType = DASHBOARD_ITEM_TYPE_MEM_USED);
    ~LGFXScreenDashboardResumeEntityMemory();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_MEMORY_H
