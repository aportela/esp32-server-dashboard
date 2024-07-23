#ifndef ESP32_SERVER_DASHBOARD_WIFI_MANAGER_H
#define ESP32_SERVER_DASHBOARD_WIFI_MANAGER_H

#include <WiFi.h>
#include <WiFi-Settings.h>

#include <cstdint>
#include <cstdio>

class WifiManager
{
private:
    static char WiFiSSID[33];
    static char WiFiPassword[65];
    static bool tryingConnection;
    static bool validConnection;
    static bool reconnect;

public:
    static void setCredentials(const char *ssid, const char *password);
    static void connect(bool reconnectIfLost = true);
    static void disconnect(void);
    static bool isConnected(void);
    static void loop(void);
};

#endif
