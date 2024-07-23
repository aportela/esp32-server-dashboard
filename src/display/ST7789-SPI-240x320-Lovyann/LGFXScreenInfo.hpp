#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H

#include "LGFXScreen.hpp"
#include "../../utils/WifiManager.hpp"

class LGFXScreenInfo : public LGFXScreen
{
private:
    bool refreshWIFILogo;
    bool refreshWIFISignalStrength;
    bool refreshWIFISignalLevelGraph;
    bool refreshWIFIData;
    bool wasConnected = false;
    long previousSignalStrength;
    WIFISignalQuality previousSignalQuality;

public:
    LGFXScreenInfo(LovyanGFX *display, FPSDebug *fpsDebug);
    ~LGFXScreenInfo();
    void refresh(bool firstRun) override;
};

#endif