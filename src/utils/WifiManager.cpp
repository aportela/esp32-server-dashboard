#include "WifiManager.hpp"

char WifiManager::WiFiSSID[WIFI_SSID_CHAR_ARR_LENGTH] = {'\0'};
char WifiManager::WiFiPassword[WIFI_PASSWORD_CHAR_ARR_LENGTH] = {'\0'};
bool WifiManager::tryingConnection = false;
bool WifiManager::validConnection = false;
bool WifiManager::reconnect = false;
char WifiManager::macAddress[MAC_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};
char WifiManager::ipAddress[IP_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};
long WifiManager::signalStrength = -91;
WIFISignalQuality WifiManager::signalQuality = WIFISignalQuality_NONE;

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
    sprintf(macAddress, "");
    sprintf(ipAddress, "");
    signalStrength = -91;
    signalQuality = WIFISignalQuality_NONE;
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
            uint8_t mac[6];
            WiFi.macAddress(mac);
            sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            IPAddress ip = WiFi.localIP();
            sprintf(ipAddress, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
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

void WifiManager::getSSID(char *ssid, size_t count)
{
    strncpy(ssid, WiFiSSID, count);
}

void WifiManager::getMacAddress(char *address, size_t count)
{
    strncpy(address, macAddress, count);
}

void WifiManager::getIPAddress(char *address, size_t count)
{
    strncpy(address, ipAddress, count);
}

long WifiManager::getSignalStrength(void)
{
    return (WiFi.RSSI());
}

WIFISignalQuality WifiManager::getSignalQuality(long signalStrength)
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
