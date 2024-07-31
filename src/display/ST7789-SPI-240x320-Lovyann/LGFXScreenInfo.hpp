#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_INFO_H

#include "LGFXScreen.hpp"

#include "../../utils/WifiManager.hpp"

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
    long previousSignalStrength;
    WIFISignalQuality previousSignalQuality;
    char previousRuntimeStr[16] = {'\0'};
    uint16_t previousFPS = 0;

    char WIFISSID[WIFI_SSID_CHAR_ARR_LENGTH] = {'\0'};
    char WIFIMacAddress[MAC_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};
    char WIFIIPAddress[IP_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};

    void refreshWIFILogo(void);
    void refreshWIFISignalStrength(void);
    void refreshWIFISignalLevelBars(void);
    void refreshWIFIData(bool forceDrawAll);
    void refreshCommonData(bool forceDrawAll);

public:
    LGFXScreenInfo(LovyanGFX *display);
    ~LGFXScreenInfo();
    void refresh(bool force = false) override;
};

#endif
