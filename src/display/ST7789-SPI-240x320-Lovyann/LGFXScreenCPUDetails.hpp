#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_CPU_DETAILS_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_CPU_DETAILS_H

#include "LGFXScreen.hpp"
#include "LGFXScreenDashboardResumeEntityCPU.hpp"
#include "../../sources/SourceData.hpp"

class LGFXScreenCPUDetails : public LGFXScreen
{
private:
    LGFXScreenDashboardResumeEntityCPU *cpuLoadBlock1 = nullptr;
    LGFXScreenDashboardResumeEntityCPU *cpuLoadBlock2 = nullptr;
    LGFXScreenDashboardResumeEntityCPU *cpuLoadBlock3 = nullptr;
    LGFXScreenDashboardResumeEntityCPU *cpuLoadBlock4 = nullptr;
    LGFXScreenDashboardResumeEntityCPU *cpuLoadBlock5 = nullptr;

    SourceData *currentSourceData;

public:
    LGFXScreenCPUDetails(LovyanGFX *display, SourceData *sourceData);
    ~LGFXScreenCPUDetails();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_CPU_DETAILS_H
