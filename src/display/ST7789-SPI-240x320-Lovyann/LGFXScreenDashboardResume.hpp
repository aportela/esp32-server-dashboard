#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H

#include "LGFXScreen.hpp"
#include "LGFXMeter.hpp"

#include "../../sources/Source.hpp"
#include "../../sources/SourceData.hpp"

class LGFXScreenDashboardResume : public LGFXScreen
{
private:
    LGFXMeter *cpuLoadMeter = nullptr;
    LGFXMeter *memoryLoadMeter = nullptr;
    LGFXMeter *cpuTemperatureLoadMeter = nullptr;
    LGFXMeter *networkDownloadBandwithLoadMeter = nullptr;
    LGFXMeter *networkUploadBandwithLoadMeter = nullptr;

    uint64_t lastCPUTimestamp = 0;
    uint64_t lastMemoryTimestamp = 0;
    uint64_t lastCPUTemperatureTimestamp = 0;
    uint64_t lastDownloadTimestamp = 0;
    uint64_t lastUploadTimestamp = 0;

    Source *currentSource;
    SourceData *currentSourceData;

    uint16_t previousFPS = 0;
    char previousRuntimeStr[16] = {'\0'};

    void refreshBottomCommonData(bool forceDrawAll);

public:
    LGFXScreenDashboardResume(LovyanGFX *display, Source *source, SourceData *sourceData);
    ~LGFXScreenDashboardResume();
    void refresh(bool force = false) override;
};

#endif
