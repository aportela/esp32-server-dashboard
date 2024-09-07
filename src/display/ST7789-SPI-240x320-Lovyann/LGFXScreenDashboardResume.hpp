#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H

#include "LGFXScreen.hpp"
#include "LGFXScreenDashboardResumeEntityCPULoad.hpp"
#include "LGFXScreenDashboardResumeEntityUsedMemory.hpp"
#include "LGFXScreenDashboardResumeEntityCPUTemperature.hpp"
#include "LGFXScreenDashboardResumeEntityNetUsedBandWidth.hpp"
#include "LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth.hpp"
#include "../../sources/SourceData.hpp"

class LGFXScreenDashboardResume : public LGFXScreen
{
private:
    LGFXScreenDashboardResumeEntityCPULoad *cpuLoadBlock = nullptr;
    LGFXScreenDashboardResumeEntityUsedMemory *usedMemoryBlock = nullptr;
    LGFXScreenDashboardResumeEntityCPUTemperature *cpuTemperatureBlock = nullptr;
    LGFXScreenDashboardResumeEntityNetUsedBandWidth *networkDownloadBandwidthBlock = nullptr;
    LGFXScreenDashboardResumeEntityNetUsedBandWidth *networkUploadBandwidthBlock = nullptr;

    SourceData *currentSourceData;

    uint16_t previousFPS = 0;
    uint16_t FPSValueXOffset = 0;

    char previousUptimeStr[16] = {'\0'};

    char hostname[SourceData::MAX_HOSTNAME_LENGTH + 1] = {'\0'};

    bool RefreshBottomCommonData(bool forceDrawAll);
    uint16_t uptimeValueXOffset = 0;

public:
    LGFXScreenDashboardResume(LovyanGFX *display, SourceData *sourceData);
    ~LGFXScreenDashboardResume();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H
