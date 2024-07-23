#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H

#include "LGFXScreen.hpp"
#include "../../utils/WifiManager.hpp"

class LGFXScreenInfo : public LGFXScreen
{
private:
    char previousRuntimeStr[16] = {'\0'};
    bool refreshWIFILogo;
    bool refreshWIFISignalStrength;
    bool refreshWIFISignalLevelGraph;
    bool refreshWIFIData;
    bool wasConnected = false;
    char WIFISSID[WIFI_SSID_CHAR_ARR_LENGTH] = {'\0'};
    char WIFIMacAddress[MAC_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};
    char WIFIIPAddress[IP_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};
    long previousSignalStrength;
    WIFISignalQuality previousSignalQuality;

public:
    LGFXScreenInfo(LovyanGFX *display, FPSDebug *fpsDebug);
    ~LGFXScreenInfo();
    void refresh(bool firstRun) override;
};

#endif