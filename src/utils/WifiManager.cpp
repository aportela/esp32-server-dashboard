#include "WifiManager.hpp"

char WifiManager::WiFiSSID[33] = {'\0'};
char WifiManager::WiFiPassword[65] = {'\0'};
bool WifiManager::tryingConnection = false;
bool WifiManager::validConnection = false;
bool WifiManager::reconnect = false;

void WifiManager::setCredentials(const char *ssid, const char *password)
{
    validConnection = false;
    strncpy(WiFiSSID, ssid, sizeof(WiFiSSID));
    strncpy(WiFiPassword, password, sizeof(WiFiPassword));
}

void WifiManager::connect(bool reconnectIfLost)
{
    reconnect = reconnectIfLost;
    if (!tryingConnection)
    {
        if (strlen(WiFiSSID) > 0)
        {
            WiFi.mode(WIFI_STA);
            WiFi.begin(WiFiSSID, WiFiPassword);
            tryingConnection = true;
        }
    }
}

void WifiManager::disconnect(void)
{
    if (tryingConnection || WiFi.status() == WL_CONNECTED)
    {
        WiFi.disconnect();
    }
    validConnection = false;
    tryingConnection = false;
}

bool WifiManager::isConnected(void)
{
    return (WiFi.status() == WL_CONNECTED);
}

void WifiManager::loop(void)
{
    if (tryingConnection)
    {
        delay(50);
        // connection success
        if (WiFi.status() == WL_CONNECTED)
        {
            tryingConnection = false;
            validConnection = true; // allow future re-connections to this network
        }
    }
    else
    {
        // connection lost: re-connect if required
        if (WiFi.status() != WL_CONNECTED && validConnection && reconnect)
        {
            WiFi.reconnect();
        }
    }
}
