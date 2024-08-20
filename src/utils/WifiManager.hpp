#ifndef ESP32_SERVER_DASHBOARD_WIFI_MANAGER_H
#define ESP32_SERVER_DASHBOARD_WIFI_MANAGER_H

#include <WiFi.h>

#include <cstdint>
#include <cstdio>

#define WIFI_SSID_CHAR_ARR_LENGTH 33
#define WIFI_PASSWORD_CHAR_ARR_LENGTH 65

#define MAC_ADDRESS_CHAR_ARR_LENGTH 19
#define IP_ADDRESS_CHAR_ARR_LENGTH 16

enum WIFISignalQuality
{
    WIFISignalQuality_NONE,
    WIFISignalQuality_WORST,
    WIFISignalQuality_BAD,
    WIFISignalQuality_NORMAL,
    WIFISignalQuality_GOOD,
    WIFISignalQuality_BEST

};

typedef void (*WifiManagerConnectionCallback)(bool connected);

class WifiManager
{
private:
    static WifiManagerConnectionCallback connectionCallback;
    static char WiFiSSID[WIFI_SSID_CHAR_ARR_LENGTH];
    static char WiFiPassword[WIFI_PASSWORD_CHAR_ARR_LENGTH];
    static bool tryingConnection;
    static bool validConnection;
    static bool reconnect;
    static char macAddress[MAC_ADDRESS_CHAR_ARR_LENGTH];
    static char ipAddress[IP_ADDRESS_CHAR_ARR_LENGTH];
    static long signalStrength;
    static WIFISignalQuality signalQuality;

public:
    static void onConnectionStatusChanged(WifiManagerConnectionCallback callback);
    static void setCredentials(const char *ssid, const char *password);
    static void connect(bool reconnectIfLost = true);
    static void disconnect(void);
    static bool isConnected(void);
    static void loop(void);
    static void getSSID(char *buffer, size_t buffer_size);
    static void getMacAddress(char *buffer, size_t buffer_size);
    static void getIPAddress(char *buffer, size_t buffer_size);
    static long getSignalStrength(void);
    static WIFISignalQuality convertToSignalQuality(long signalStrength);
};

#endif
