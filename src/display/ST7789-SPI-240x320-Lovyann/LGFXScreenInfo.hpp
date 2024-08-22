#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H

#include "LGFXScreen.hpp"

#include "../../utils/WiFiManager.hpp"

using namespace aportela::microcontroller::utils;

class LGFXScreenInfo : public LGFXScreen
{
private:
    // flags to "refresh only when required" to optimize redraw speed
    bool wasConnected = false;
    bool WIFILogoChanged;
    bool WIFISignalStrengthChanged;
    bool WIFISignalLevelBarsChanged;
    bool WIFIDataChanged;

    // properties for tracking changes
    long previousWiFiSignalStrength;
    WiFiSignalQuality previousWiFiSignalQuality;
    char previousRuntimeStr[16] = {'\0'};
    uint16_t previousFPS = 0;

    char WIFISSID[WiFiManager::MAX_SSID_LENGTH + 1] = {'\0'};
    char WIFIMacAddress[WiFiManager::MAC_ADDRESS_LENGTH + 1] = {'\0'};
    char WIFIIPAddress[WiFiManager::IP_ADDRESS_LENGTH + 1] = {'\0'};

    void refreshWIFILogo(void);
    void refreshWIFISignalStrength(void);
    void refreshWIFISignalLevelBars(void);
    void refreshWIFIData(bool forceDrawAll);
    bool refreshCommonData(bool forceDrawAll);

public:
    LGFXScreenInfo(LovyanGFX *display);
    ~LGFXScreenInfo();
    bool refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H
