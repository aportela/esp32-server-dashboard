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
    bool wiFiLogoChanged;
    bool wiFiSignalStrengthChanged;
    bool wiFiSignalLevelBarsChanged;
    bool wiFiDataChanged;

    // properties for tracking changes
    long previousWiFiSignalStrength;
    WIFI_SIGNAL_QUALITY previousWIFI_SIGNAL_QUALITY;
    char previousRuntimeStr[16] = {'\0'};
    uint16_t previousFPS = 0;

    char wiFiSSID[WiFiManager::MAX_SSID_LENGTH + 1] = {'\0'};
    char wiFiMacAddress[WiFiManager::MAC_ADDRESS_LENGTH + 1] = {'\0'};
    char wiFiIPAddress[WiFiManager::IP_ADDRESS_LENGTH + 1] = {'\0'};

    void RefreshWIFILogo(void);
    void RefreshWIFISignalStrength(void);
    void RefreshWIFISignalLevelBars(void);
    void RefreshWIFIData(bool forceDrawAll);
    bool RefreshCommonData(bool forceDrawAll);

public:
    LGFXScreenInfo(LovyanGFX *display);
    ~LGFXScreenInfo();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H
