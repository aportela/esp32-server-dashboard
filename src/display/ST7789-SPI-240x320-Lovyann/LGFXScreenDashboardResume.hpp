#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H

#include "LGFXScreen.hpp"
#include "LGFXScreenDashboardResumeEntityCPU.hpp"
#include "LGFXScreenDashboardResumeEntityMemory.hpp"
#include "LGFXScreenDashboardResumeEntityCPUTemperature.hpp"
#include "LGFXScreenDashboardResumeEntityNetUsedBandWidth.hpp"
#include "LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth.hpp"
#include "../../sources/SourceData.hpp"
#include "../DashboardItemType.hpp"

class LGFXScreenDashboardResume : public LGFXScreen
{
private:
    LGFXScreenDashboardResumeEntity *block1 = nullptr;
    LGFXScreenDashboardResumeEntity *block2 = nullptr;
    LGFXScreenDashboardResumeEntity *block3 = nullptr;
    LGFXScreenDashboardResumeEntity *block4 = nullptr;
    LGFXScreenDashboardResumeEntity *block5 = nullptr;

    SourceData *currentSourceData;

    uint16_t previousFPS = 0;
    uint16_t FPSValueXOffset = 0;

    char previousUptimeStr[16] = {'\0'};

    char hostname[SourceData::MAX_HOSTNAME_LENGTH + 1] = {'\0'};

    bool RefreshBottomCommonData(bool forceDrawAll);
    uint16_t uptimeValueXOffset = 0;
    uint8_t dashboardIndex = 0;

public:
    LGFXScreenDashboardResume(LovyanGFX *display, SourceData *sourceData, const uint8_t dashboardIndex = 0, const DASHBOARD_ITEM_TYPE items[DASHBOARD_ITEM_COUNT] = {0});
    ~LGFXScreenDashboardResume();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H
