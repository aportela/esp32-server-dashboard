#ifndef ESP32_SERVER_DASHBOARD_SETTINGS_H
#define ESP32_SERVER_DASHBOARD_SETTINGS_H

#include <cstdint>
#include <cstddef>
#include <Preferences.h>

class Settings
{
private:
    Preferences *preferences;

    uint64_t getBigUnsignedIntegerValue(const char *key, uint64_t defaultValue = 0);
    void setBigIntegerValue(const char *key, uint64_t value);
    void getValue(const char *key, char *value, size_t count);
    void setValue(const char *key, const char *value);
    void deleteKey(const char *key);

public:
    Settings(void);
    ~Settings();
    void clear(void);

    void getWIFISSID(char *ssid, size_t count);
    void setWIFISSID(const char *ssid);
    void getWIFIPassword(char *password, size_t count);
    void setWIFIPassword(const char *password);

    void getMQTTTelegrafURI(char *uri, size_t count);
    void setMQTTTelegrafURI(const char *uri);

    void getMQTTTelegrafGlobalTopic(char *topic, size_t count);
    void setMQTTTelegrafGlobalTopic(const char *topic);

    uint64_t getTotalMemoryBytes();
    void setTotalMemoryBytes(uint64_t totalBytes = 0);
};

#endif
