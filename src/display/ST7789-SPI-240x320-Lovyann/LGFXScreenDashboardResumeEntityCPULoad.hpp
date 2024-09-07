#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_LOAD_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_LOAD_H

#include "LGFXScreenDashboardResumeEntity.hpp"

enum CPU_USAGE_TYPE
{
    CPU_USAGE_TYPE_CPU_LOAD = 0,
    CPU_USAGE_TYPE_CPU_SYSTEM = 1,
    CPU_USAGE_TYPE_CPU_USER = 2,
    CPU_USAGE_TYPE_CPU_IDLE = 3,
    CPU_USAGE_TYPE_CPU_NICE = 4,
    CPU_USAGE_TYPE_CPU_IOWAIT = 5,
    CPU_USAGE_TYPE_CPU_IRQ = 6,
    CPU_USAGE_TYPE_CPU_SOFT_IRQ = 7,
    CPU_USAGE_TYPE_CPU_GUEST = 8,
    CPU_USAGE_TYPE_CPU_GUEST_NICE = 9,
    CPU_USAGE_TYPE_CPU_STEAL = 10,
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
