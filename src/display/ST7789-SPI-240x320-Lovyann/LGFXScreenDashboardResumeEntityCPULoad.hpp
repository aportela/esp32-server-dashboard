#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_LOAD_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_LOAD_H

#include "LGFXScreenDashboardResumeEntity.hpp"

enum CPU_USAGE_TYPE
{
    CPU_USAGE_TYPE_CPU_LOAD = 0,
};

class LGFXScreenDashboardResumeEntityCPULoad : public LGFXScreenDashboardResumeEntity
{
private:
    uint64_t timestamp = 0;
    float value = 0;
    CPU_USAGE_TYPE cpuUsageType = CPU_USAGE_TYPE_CPU_LOAD;

public:
    LGFXScreenDashboardResumeEntityCPULoad(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, CPU_USAGE_TYPE cpuUsageType = CPU_USAGE_TYPE_CPU_LOAD);
    ~LGFXScreenDashboardResumeEntityCPULoad();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_LOAD_H
