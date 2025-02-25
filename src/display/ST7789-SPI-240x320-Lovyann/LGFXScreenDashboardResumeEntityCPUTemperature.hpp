#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_TEMPERATURE_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_TEMPERATURE_H

#include "LGFXScreenDashboardResumeEntity.hpp"

class LGFXScreenDashboardResumeEntityCPUTemperature : public LGFXScreenDashboardResumeEntity
{
private:
    uint64_t timestamp = 0;
    float value = 0;

public:
    LGFXScreenDashboardResumeEntityCPUTemperature(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset);
    ~LGFXScreenDashboardResumeEntityCPUTemperature();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_CPU_TEMPERATURE_H
