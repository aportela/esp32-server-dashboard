#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_LOAD_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_LOAD_H

#include "LGFXScreenDashboardResumeEntity.hpp"
#include "../DashboardItemType.hpp"

class LGFXScreenDashboardResumeEntityCPU : public LGFXScreenDashboardResumeEntity
{
private:
    uint64_t timestamp = 0;
    float value = 0;
    DASHBOARD_ITEM_TYPE cpuUsageType = DASHBOARD_ITEM_TYPE_CPU_LOAD;

public:
    LGFXScreenDashboardResumeEntityCPU(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, DASHBOARD_ITEM_TYPE cpuUsageType = DASHBOARD_ITEM_TYPE_CPU_LOAD);
    ~LGFXScreenDashboardResumeEntityCPU();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_LOAD_H
