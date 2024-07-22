#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H

#include "LGFXScreen.hpp"

class LGFXScreenInfo : public LGFXScreen
{
private:
    bool refreshWIFILogo;
    bool refreshWIFISignalStrength;
    bool refreshWIFISignalLevelGraph;

public:
    LGFXScreenInfo(LovyanGFX *display, FPSDebug *fpsDebug, SystemStatus *sysStats);
    ~LGFXScreenInfo();
    void refresh(void) override;
};

#endif