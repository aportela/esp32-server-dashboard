#ifndef ESP32_SERVER_DASHBOARD_WIFI_MANAGER_H
#define ESP32_SERVER_DASHBOARD_WIFI_MANAGER_H

#include <WiFi.h>

#include <cstdint>
#include <cstdio>

#define SSID_CHAR_ARR_LENGTH 33
#define PASSWORD_CHAR_ARR_LENGTH 65

#define MAC_ADDRESS_CHAR_ARR_LENGTH 19
#define IP_ADDRESS_CHAR_ARR_LENGTH 16

enum WiFiSignalQuality
{
    WiFiSignalQuality_NONE,
    WiFiSignalQuality_WORST,
    WiFiSignalQuality_BAD,
    WiFiSignalQuality_NORMAL,
    WiFiSignalQuality_GOOD,
    WiFiSignalQuality_BEST

};

typedef void (*WiFiManagerConnectionCallback)(bool connected);

class WiFiManager
{
private:
    static WiFiManagerConnectionCallback connectionCallback;
    static char WiFiSSID[SSID_CHAR_ARR_LENGTH];
    static char WiFiPassword[PASSWORD_CHAR_ARR_LENGTH];
    static bool tryingConnection;
    static bool validConnection;
    static bool reconnect;
    static char macAddress[MAC_ADDRESS_CHAR_ARR_LENGTH];
    static char ipAddress[IP_ADDRESS_CHAR_ARR_LENGTH];
    static long signalStrength;
    static WiFiSignalQuality signalQuality;

public:
    static const uint8_t MAX_SSID_LENGTH = SSID_CHAR_ARR_LENGTH - 1;
    static const uint8_t MAX_PASSWORD_LENGTH = PASSWORD_CHAR_ARR_LENGTH - 1;
    static const uint8_t MAC_ADDRESS_LENGTH = MAC_ADDRESS_CHAR_ARR_LENGTH - 1;
    static const uint8_t IP_ADDRESS_LENGTH = IP_ADDRESS_CHAR_ARR_LENGTH - 1;

    static void onConnectionStatusChanged(WiFiManagerConnectionCallback callback);
    static void setCredentials(const char *ssid, const char *password);
    static void connect(bool reconnectIfLost = true);
    static void disconnect(void);
    static bool isConnected(void);
    static void loop(void);
    static void getSSID(char *buffer, size_t buffer_size);
    static void getMacAddress(char *buffer, size_t buffer_size);
    static void getIPAddress(char *buffer, size_t buffer_size);
    static long getSignalStrength(void);
    static WiFiSignalQuality convertToSignalQuality(long signalStrength);
};

#endif
