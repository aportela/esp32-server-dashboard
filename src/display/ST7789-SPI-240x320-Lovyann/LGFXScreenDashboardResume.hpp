#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_H

#include "LGFXScreen.hpp"
#include "LGFXMeter.hpp"

#include "../../sources/Source.hpp"

class LGFXScreenDashboardResume : public LGFXScreen
{
private:
    LGFXMeter *cpuLoadMeter = nullptr;
    LGFXMeter *memoryLoadMeter = nullptr;
    LGFXMeter *cpuTemperatureLoadMeter = nullptr;
    LGFXMeter *networkDownloadBandwithLoadMeter = nullptr;
    LGFXMeter *networkUploadBandwithLoadMeter = nullptr;

    Source *currentSource;

    uint16_t previousFPS = 0;

    void refreshBottomCommonData(bool forceDrawAll);

public:
    LGFXScreenDashboardResume(LovyanGFX *display, Source *source);
    ~LGFXScreenDashboardResume();
    void refresh(bool force = false) override;
};

#endif
