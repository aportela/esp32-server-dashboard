#ifndef APORTELA_MICROCONTROLLER_UTILS_WIFIMANAGER_H
#define APORTELA_MICROCONTROLLER_UTILS_WIFIMANAGER_H

#include <WiFi.h>
#include <cstdint>
#include <cstdio>

#define SSID_CHAR_ARR_LENGTH 33
#define PASSWORD_CHAR_ARR_LENGTH 65

#define MAC_ADDRESS_CHAR_ARR_LENGTH 19
#define IP_ADDRESS_CHAR_ARR_LENGTH 16

namespace aportela::microcontroller::utils
{
    enum WIFI_SIGNAL_QUALITY
    {
        WIFI_SIGNAL_QUALITY_NONE,
        WIFI_SIGNAL_QUALITY_WORST,
        WIFI_SIGNAL_QUALITY_BAD,
        WIFI_SIGNAL_QUALITY_NORMAL,
        WIFI_SIGNAL_QUALITY_GOOD,
        WIFI_SIGNAL_QUALITY_BEST
    };

    typedef void (*WiFiManagerConnectionCallback)(bool connected);

    class WiFiManager
    {
    private:
        static WiFiManagerConnectionCallback connectionCallback;
        static char wiFiSSID[SSID_CHAR_ARR_LENGTH];
        static char wiFiPassword[PASSWORD_CHAR_ARR_LENGTH];
        static bool tryingConnection;
        static bool validConnection;
        static bool reconnect;
        static char macAddress[MAC_ADDRESS_CHAR_ARR_LENGTH];
        static char ipAddress[IP_ADDRESS_CHAR_ARR_LENGTH];
        static long signalStrength;
        static WIFI_SIGNAL_QUALITY signalQuality;

    public:
        static const uint8_t MAX_SSID_LENGTH = SSID_CHAR_ARR_LENGTH - 1;
        static const uint8_t MAX_PASSWORD_LENGTH = PASSWORD_CHAR_ARR_LENGTH - 1;
        static const uint8_t MAC_ADDRESS_LENGTH = MAC_ADDRESS_CHAR_ARR_LENGTH - 1;
        static const uint8_t IP_ADDRESS_LENGTH = IP_ADDRESS_CHAR_ARR_LENGTH - 1;

        static void OnConnectionStatusChanged(WiFiManagerConnectionCallback callback);
        static void SetCredentials(const char *ssid, const char *password);
        static void Connect(bool reconnectIfLost = true);
        static void Disconnect(void);
        static bool IsConnected(void);
        static void Loop(void);
        static void GetSSID(char *buffer, size_t bufferSize);
        static void GetMacAddress(char *buffer, size_t bufferSize);
        static void GetIPAddress(char *buffer, size_t bufferSize);
        static long GetSignalStrength(void);
        static WIFI_SIGNAL_QUALITY ConvertToSignalQuality(long signalStrength);
    };
}

#endif // APORTELA_MICROCONTROLLER_UTILS_WIFIMANAGER_H
