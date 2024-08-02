#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H

#include "LGFXScreen.hpp"
#include "LGFXScreenDashboardResumeEntityCPULoad.hpp"
#include "LGFXScreenDashboardResumeEntityUsedMemory.hpp"
#include "LGFXScreenDashboardResumeEntityCPUTemperature.hpp"
#include "LGFXScreenDashboardResumeEntityNetUsedBandWidth.hpp"
#include "../../sources/SourceData.hpp"

class LGFXScreenDashboardResume : public LGFXScreen
{
private:
    LGFXScreenDashboardResumeEntityCPULoad *dre = nullptr;
    LGFXScreenDashboardResumeEntityUsedMemory *dre2 = nullptr;
    LGFXScreenDashboardResumeEntityCPUTemperature *dre3 = nullptr;
    LGFXScreenDashboardResumeEntityNetUsedBandWidth *dre4 = nullptr;
    LGFXScreenDashboardResumeEntityNetUsedBandWidth *dre5 = nullptr;

    uint64_t lastCPUTimestamp = 0;
    uint64_t lastMemoryTimestamp = 0;
    uint64_t lastCPUTemperatureTimestamp = 0;
    uint64_t lastDownloadTimestamp = 0;
    uint64_t lastUploadTimestamp = 0;

    SourceData *currentSourceData;

    uint16_t previousFPS = 0;
    char previousUptimeStr[16] = {'\0'};

    void refreshBottomCommonData(bool forceDrawAll);

public:
    LGFXScreenDashboardResume(LovyanGFX *display, SourceData *sourceData);
    ~LGFXScreenDashboardResume();
    void refresh(bool force = false) override;
};

#endif
