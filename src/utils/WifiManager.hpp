#ifndef ESP32_SERVER_DASHBOARD_WIFI_MANAGER_H
#define ESP32_SERVER_DASHBOARD_WIFI_MANAGER_H

#include <WiFi.h>
#include <WiFi-Settings.h>

#include <cstdint>
#include <cstdio>

enum WIFISignalQuality
{
    WIFISignalQuality_NONE,
    WIFISignalQuality_WORST,
    WIFISignalQuality_BAD,
    WIFISignalQuality_NORMAL,
    WIFISignalQuality_GOOD,
    WIFISignalQuality_BEST

};

class WifiManager
{
private:
    static char WiFiSSID[33];
    static char WiFiPassword[65];
    static bool tryingConnection;
    static bool validConnection;
    static bool reconnect;
    static char macAddress[19];
    static char ipAddress[16];
    static long signalStrength;
    static WIFISignalQuality signalQuality;

public:
    static void setCredentials(const char *ssid, const char *password);
    static void connect(bool reconnectIfLost = true);
    static void disconnect(void);
    static bool isConnected(void);
    static void loop(void);
    static void getSSID(char *ssid, size_t count);
    static void getMacAddress(char *address, size_t count);
    static void getIPAddress(char *address, size_t count);
    static long getSignalStrength(void);
    static WIFISignalQuality getSignalQuality(long signalStrength);
};

#endif
