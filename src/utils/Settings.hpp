#ifndef ESP32_SERVER_DASHBOARD_SETTINGS_H
#define ESP32_SERVER_DASHBOARD_SETTINGS_H

#include <cstddef>
#include <Preferences.h>

class Settings
{
private:
    static Preferences preferences;

    static int8_t getSmallSignedIntegerValue(const char *key, int8_t defaultValue = 0);
    static bool setSmallSignedIntegerValue(const char *key, int8_t value = 0);
    static uint64_t getBigUnsignedIntegerValue(const char *key, uint64_t defaultValue = 0);
    static bool setBigUnsigedIntegerValue(const char *key, uint64_t value = 0);
    static float getFloatValue(const char *key, float defaultValue = 0.0f);
    static bool setFloatValue(const char *key, float value = 0.0f);
    static void getValue(const char *key, char *value, size_t count);
    static bool setValue(const char *key, const char *value);
    static bool deleteKey(const char *key);

public:
    static bool clear(void);

    static void getWIFISSID(char *ssid, size_t count);
    static bool setWIFISSID(const char *ssid);
    static void getWIFIPassword(char *password, size_t count);
    static bool setWIFIPassword(const char *password);

    static void getMQTTTelegrafURI(char *uri, size_t count);
    static bool setMQTTTelegrafURI(const char *uri);

    static void getMQTTTelegrafGlobalTopic(char *topic, size_t count);
    static bool setMQTTTelegrafGlobalTopic(const char *topic);

    static uint64_t getTotalMemoryBytes();
    static bool setTotalMemoryBytes(uint64_t totalBytes = 0);

    static float getMinCPUTemperature();
    static bool setMinCPUTemperature(float celsius = 0.0f);

    static float getMaxCPUTemperature();
    static bool setMaxCPUTemperature(float celsius = 0.0f);

    static uint64_t getMaxDownloadBandwidthBytes();
    static bool setMaxDownloadBandwidthBytes(uint64_t totalBytes = 0);

    static uint64_t getMaxUploadBandwidthBytes();
    static bool setMaxUploadBandwidthBytes(uint64_t totalBytes = 0);
};

#endif
