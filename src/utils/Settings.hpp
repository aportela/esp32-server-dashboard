#ifndef ESP32_SERVER_DASHBOARD_SETTINGS_H
#define ESP32_SERVER_DASHBOARD_SETTINGS_H

#include <cstddef>
#include <Preferences.h>

class Settings
{
private:
    Preferences *preferences;

    int8_t getSmallSignedIntegerValue(const char *key, int8_t defaultValue = 0);
    bool setSmallSignedIntegerValue(const char *key, int8_t value = 0);
    uint64_t getBigUnsignedIntegerValue(const char *key, uint64_t defaultValue = 0);
    bool setBigUnsigedIntegerValue(const char *key, uint64_t value = 0);
    float getFloatValue(const char *key, float defaultValue = 0.0f);
    bool setFloatValue(const char *key, float value = 0.0f);
    void getValue(const char *key, char *value, size_t count);
    bool setValue(const char *key, const char *value);
    bool deleteKey(const char *key);

public:
    Settings(void);
    ~Settings();

    bool clear(void);

    void getWIFISSID(char *ssid, size_t count);
    bool setWIFISSID(const char *ssid);
    void getWIFIPassword(char *password, size_t count);
    bool setWIFIPassword(const char *password);

    void getMQTTTelegrafURI(char *uri, size_t count);
    bool setMQTTTelegrafURI(const char *uri);

    void getMQTTTelegrafGlobalTopic(char *topic, size_t count);
    bool setMQTTTelegrafGlobalTopic(const char *topic);

    uint64_t getMaxDownloadBandwidthBytes();
    bool setMaxDownloadBandwidthBytes(uint64_t totalBytes = 0);

    uint64_t getMaxUploadBandwidthBytes();
    bool setMaxUploadBandwidthBytes(uint64_t totalBytes = 0);

    void getNetworkInterfaceId(char *id, size_t count);
    bool setNetworkInterfaceId(const char *id);
};

#endif
