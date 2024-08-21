#ifndef ESP32_SERVER_DASHBOARD_CUSTOM_SETTINGS_H
#define ESP32_SERVER_DASHBOARD_CUSTOM_SETTINGS_H

#include "utils/Settings.hpp"
#include "src/display/ScreenType.hpp"

class CustomSettings : public Settings
{
public:
    CustomSettings();
    ~CustomSettings();

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

    void getHostname(char *name, size_t count);
    bool setHostname(const char *name);

    ScreenType getDefaultScreen(ScreenType defaultScreen = ST_NONE);
    bool setDefaultScreen(ScreenType defaultScreen);
};

#endif
