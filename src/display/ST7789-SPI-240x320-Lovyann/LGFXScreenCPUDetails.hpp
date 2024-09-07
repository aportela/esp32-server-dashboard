#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_CPU_DETAILS_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_CPU_DETAILS_H

#include "LGFXScreen.hpp"
#include "LGFXScreenDashboardResumeEntityCPULoad.hpp"
#include "../../sources/SourceData.hpp"

class LGFXScreenCPUDetails : public LGFXScreen
{
private:
    LGFXScreenDashboardResumeEntityCPULoad *cpuLoadBlock1 = nullptr;
    LGFXScreenDashboardResumeEntityCPULoad *cpuLoadBlock2 = nullptr;
    LGFXScreenDashboardResumeEntityCPULoad *cpuLoadBlock3 = nullptr;
    LGFXScreenDashboardResumeEntityCPULoad *cpuLoadBlock4 = nullptr;
    LGFXScreenDashboardResumeEntityCPULoad *cpuLoadBlock5 = nullptr;

    SourceData *currentSourceData;

public:
    LGFXScreenCPUDetails(LovyanGFX *display, SourceData *sourceData);
    ~LGFXScreenCPUDetails();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_CPU_DETAILS_H
