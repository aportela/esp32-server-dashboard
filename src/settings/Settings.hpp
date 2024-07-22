#ifndef ESP32_SERVER_DASHBOARD_SETTINGS_H
#define ESP32_SERVER_DASHBOARD_SETTINGS_H

#include <stdint.h>
#include <Preferences.h>

class Settings
{
private:
    Preferences *preferences;

    char WIFI_SSID[32];
    char WIFI_PASSWORD[64]; // will be enought ?

public:
    Settings(void);
    ~Settings();
    void clear(void);
};

#endif
