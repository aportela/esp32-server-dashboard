#include "WifiManager.hpp"

WifiManagerConnectionCallback WifiManager::connectionCallback = nullptr;
char WifiManager::WiFiSSID[SSID_CHAR_ARR_LENGTH] = {'\0'};
char WifiManager::WiFiPassword[PASSWORD_CHAR_ARR_LENGTH] = {'\0'};
bool WifiManager::tryingConnection = false;
bool WifiManager::validConnection = false;
bool WifiManager::reconnect = false;
char WifiManager::macAddress[MAC_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};
char WifiManager::ipAddress[IP_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};
long WifiManager::signalStrength = -91;
WIFISignalQuality WifiManager::signalQuality = WIFISignalQuality_NONE;

void WifiManager::onConnectionStatusChanged(WifiManagerConnectionCallback callback)
{
    if (callback != nullptr)
    {
        WifiManager::connectionCallback = callback;
    }
}

void WifiManager::setCredentials(const char *ssid, const char *password)
{
    WifiManager::validConnection = false;
    strncpy(WifiManager::WiFiSSID, ssid, sizeof(WifiManager::WiFiSSID));
    strncpy(WifiManager::WiFiPassword, password, sizeof(WifiManager::WiFiPassword));
}

void WifiManager::connect(bool reconnectIfLost)
{
    WifiManager::reconnect = reconnectIfLost;
    if (!WifiManager::tryingConnection)
    {
        if (strlen(WifiManager::WiFiSSID) > 0)
        {
            WiFi.mode(WIFI_STA);
            WiFi.begin(WifiManager::WiFiSSID, WifiManager::WiFiPassword);
            WifiManager::tryingConnection = true;
        }
    }
}

void WifiManager::disconnect(void)
{
    if (WifiManager::tryingConnection || WiFi.status() == WL_CONNECTED)
    {
        WiFi.disconnect();
    }
    WifiManager::validConnection = false;
    WifiManager::tryingConnection = false;
    std::sprintf(WifiManager::macAddress, "");
    std::sprintf(WifiManager::ipAddress, "");
    WifiManager::signalStrength = -91;
    WifiManager::signalQuality = WIFISignalQuality_NONE;
    if (WifiManager::connectionCallback != nullptr)
    {
        WifiManager::connectionCallback(false);
    }
}

bool WifiManager::isConnected(void)
{
    return (WiFi.status() == WL_CONNECTED);
}

void WifiManager::loop(void)
{
    if (WifiManager::tryingConnection)
    {
        delay(50);
        // connection success
        if (WiFi.status() == WL_CONNECTED)
        {
            uint8_t mac[6];
            WiFi.macAddress(mac);
            std::sprintf(WifiManager::macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            IPAddress ip = WiFi.localIP();
            std::sprintf(WifiManager::ipAddress, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
            WifiManager::tryingConnection = false;
            WifiManager::validConnection = true; // allow future re-connections to this network
            if (WifiManager::connectionCallback != nullptr)
            {
                WifiManager::connectionCallback(true);
            }
        }
    }
    else
    {
        // connection lost: re-connect if required
        if (WiFi.status() != WL_CONNECTED && WifiManager::validConnection && WifiManager::reconnect)
        {
            WiFi.disconnect();
            WifiManager::tryingConnection = true;
            WiFi.reconnect();
        }
    }
}

void WifiManager::getSSID(char *buffer, size_t buffer_size)
{
    strncpy(buffer, WifiManager::WiFiSSID, buffer_size);
}

void WifiManager::getMacAddress(char *buffer, size_t buffer_size)
{
    strncpy(buffer, WifiManager::macAddress, buffer_size);
}

void WifiManager::getIPAddress(char *buffer, size_t buffer_size)
{
    strncpy(buffer, WifiManager::ipAddress, buffer_size);
}

long WifiManager::getSignalStrength(void)
{
    return (WiFi.RSSI());
}

WIFISignalQuality WifiManager::convertToSignalQuality(long signalStrength)
{
    if (signalStrength < -90)
    {
        return (WIFISignalQuality_NONE);
    }
    else if (signalStrength < -80)
    {
        return (WIFISignalQuality_WORST);
    }
    else if (signalStrength < -70)
    {
        return (WIFISignalQuality_BAD);
    }
    else if (signalStrength < -67)
    {
        return (WIFISignalQuality_NORMAL);
    }
    else if (signalStrength < -30)
    {
        return (WIFISignalQuality_GOOD);
    }
    else
    {
        return (WIFISignalQuality_BEST);
    }
}
