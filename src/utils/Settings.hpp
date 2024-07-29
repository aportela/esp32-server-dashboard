#ifndef ESP32_SERVER_DASHBOARD_SETTINGS_H
#define ESP32_SERVER_DASHBOARD_SETTINGS_H

#include <cstdint>
#include <cstddef>
#include <Preferences.h>

class Settings
{
private:
    Preferences *preferences;

    void getValue(const char *key, char *value, size_t count);
    void setValue(const char *key, const char *value);

public:
    Settings(void);
    ~Settings();
    void clear(void);

    void getWIFISSID(char *ssid, size_t count);
    void setWIFISSID(const char *ssid);
    void getWIFIPassword(char *password, size_t count);
    void setWIFIPassword(const char *password);
};

#endif
