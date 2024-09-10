#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_LOAD_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_LOAD_H

#include "LGFXScreenDashboardResumeEntity.hpp"

enum CPU_USAGE_TYPE
{
    CPU_USAGE_TYPE_LOAD = 0,
    CPU_USAGE_TYPE_USER = 1,
    CPU_USAGE_TYPE_SYSTEM = 2,
    CPU_USAGE_TYPE_IDLE = 3,
    CPU_USAGE_TYPE_ACTIVE = 4,
    CPU_USAGE_TYPE_NICE = 5,
    CPU_USAGE_TYPE_IOWAIT = 6,
    CPU_USAGE_TYPE_IRQ = 7,
    CPU_USAGE_TYPE_SOFT_IRQ = 8,
    CPU_USAGE_TYPE_STEAL = 9,
    CPU_USAGE_TYPE_GUEST = 10,
    CPU_USAGE_TYPE_GUEST_NICE = 11,
};

class LGFXScreenDashboardResumeEntityCPU : public LGFXScreenDashboardResumeEntity
{
private:
    uint64_t timestamp = 0;
    float value = 0;
    CPU_USAGE_TYPE cpuUsageType = CPU_USAGE_TYPE_LOAD;

public:
    LGFXScreenDashboardResumeEntityCPU(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, CPU_USAGE_TYPE cpuUsageType = CPU_USAGE_TYPE_LOAD);
    ~LGFXScreenDashboardResumeEntityCPU();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_LOAD_H
