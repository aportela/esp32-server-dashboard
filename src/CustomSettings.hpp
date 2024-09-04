#ifndef ESP32_SERVER_DASHBOARD_CUSTOM_SETTINGS_H
#define ESP32_SERVER_DASHBOARD_CUSTOM_SETTINGS_H

#include "utils/Settings.hpp"
#include "src/display/ScreenType.hpp"

using namespace aportela::microcontroller::utils;

class CustomSettings : public Settings
{
public:
    CustomSettings();
    ~CustomSettings();

    void GetWIFISSID(char *ssid, size_t count);
    bool SetWIFISSID(const char *ssid);
    void GetWIFIPassword(char *password, size_t count);
    bool SetWIFIPassword(const char *password);

    void GetMQTTTelegrafURI(char *uri, size_t count);
    bool SetMQTTTelegrafURI(const char *uri);

    void GetMQTTUsername(char *username, size_t count);
    bool SetMQTTUsername(const char *username);
    void GetMQTTPassword(char *password, size_t count);
    bool SetMQTTPassword(const char *password);
    void GetMQTTTelegrafGlobalTopic(char *topic, size_t count);
    bool SetMQTTTelegrafGlobalTopic(const char *topic);

    uint64_t GetMaxDownloadBandwidthBytes();
    bool SetMaxDownloadBandwidthBytes(uint64_t totalBytes = 0);

    uint64_t GetMaxUploadBandwidthBytes();
    bool SetMaxUploadBandwidthBytes(uint64_t totalBytes = 0);

    void GetNetworkInterfaceId(char *id, size_t count);
    bool SetNetworkInterfaceId(const char *id);

    void GetHostname(char *name, size_t count);
    bool SetHostname(const char *name);

    bool GetScreenMirrorFlipVertical();
    bool SetScreenMirrorFlipVertical(bool mirrorFlipVertical = false);

    SCREEN_TYPE GetDefaultScreen(SCREEN_TYPE defaultScreen = SCREEN_TYPE_NONE);
    bool SetDefaultScreen(SCREEN_TYPE defaultScreen);
};

#endif // ESP32_SERVER_DASHBOARD_CUSTOM_SETTINGS_H
