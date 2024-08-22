#include "WiFiManager.hpp"

namespace aportela::microcontroller::utils
{
    WiFiManagerConnectionCallback WiFiManager::connectionCallback = nullptr;
    char WiFiManager::WiFiSSID[SSID_CHAR_ARR_LENGTH] = {'\0'};
    char WiFiManager::WiFiPassword[PASSWORD_CHAR_ARR_LENGTH] = {'\0'};
    bool WiFiManager::tryingConnection = false;
    bool WiFiManager::validConnection = false;
    bool WiFiManager::reconnect = false;
    char WiFiManager::macAddress[MAC_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};
    char WiFiManager::ipAddress[IP_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};
    long WiFiManager::signalStrength = -91;
    WiFiSignalQuality WiFiManager::signalQuality = WiFiSignalQuality_NONE;

    void WiFiManager::onConnectionStatusChanged(WiFiManagerConnectionCallback callback)
    {
        if (callback != nullptr)
        {
            WiFiManager::connectionCallback = callback;
        }
    }

    void WiFiManager::setCredentials(const char *ssid, const char *password)
    {
        WiFiManager::validConnection = false;
        strncpy(WiFiManager::WiFiSSID, ssid, sizeof(WiFiManager::WiFiSSID));
        strncpy(WiFiManager::WiFiPassword, password, sizeof(WiFiManager::WiFiPassword));
    }

    void WiFiManager::connect(bool reconnectIfLost)
    {
        WiFiManager::reconnect = reconnectIfLost;
        if (!WiFiManager::tryingConnection)
        {
            if (strlen(WiFiManager::WiFiSSID) > 0)
            {
                WiFi.mode(WIFI_STA);
                WiFi.begin(WiFiManager::WiFiSSID, WiFiManager::WiFiPassword);
                WiFiManager::tryingConnection = true;
            }
        }
    }

    void WiFiManager::disconnect(void)
    {
        if (WiFiManager::tryingConnection || WiFi.status() == WL_CONNECTED)
        {
            WiFi.disconnect();
        }
        WiFiManager::validConnection = false;
        WiFiManager::tryingConnection = false;
        std::sprintf(WiFiManager::macAddress, "");
        std::sprintf(WiFiManager::ipAddress, "");
        WiFiManager::signalStrength = -91;
        WiFiManager::signalQuality = WiFiSignalQuality_NONE;
        if (WiFiManager::connectionCallback != nullptr)
        {
            WiFiManager::connectionCallback(false);
        }
    }

    bool WiFiManager::isConnected(void)
    {
        return (WiFi.status() == WL_CONNECTED);
    }

    void WiFiManager::loop(void)
    {
        if (WiFiManager::tryingConnection)
        {
            delay(50);
            // connection success
            if (WiFi.status() == WL_CONNECTED)
            {
                uint8_t mac[6];
                WiFi.macAddress(mac);
                std::sprintf(WiFiManager::macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                IPAddress ip = WiFi.localIP();
                std::sprintf(WiFiManager::ipAddress, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
                WiFiManager::tryingConnection = false;
                WiFiManager::validConnection = true; // allow future re-connections to this network
                if (WiFiManager::connectionCallback != nullptr)
                {
                    WiFiManager::connectionCallback(true);
                }
            }
        }
        else
        {
            // connection lost: re-connect if required
            if (WiFi.status() != WL_CONNECTED && WiFiManager::validConnection && WiFiManager::reconnect)
            {
                WiFi.disconnect();
                WiFiManager::tryingConnection = true;
                WiFi.reconnect();
            }
        }
    }

    void WiFiManager::getSSID(char *buffer, size_t buffer_size)
    {
        strncpy(buffer, WiFiManager::WiFiSSID, buffer_size);
    }

    void WiFiManager::getMacAddress(char *buffer, size_t buffer_size)
    {
        strncpy(buffer, WiFiManager::macAddress, buffer_size);
    }

    void WiFiManager::getIPAddress(char *buffer, size_t buffer_size)
    {
        strncpy(buffer, WiFiManager::ipAddress, buffer_size);
    }

    long WiFiManager::getSignalStrength(void)
    {
        return (WiFi.RSSI());
    }

    WiFiSignalQuality WiFiManager::convertToSignalQuality(long signalStrength)
    {
        if (signalStrength < -90)
        {
            return (WiFiSignalQuality_NONE);
        }
        else if (signalStrength < -80)
        {
            return (WiFiSignalQuality_WORST);
        }
        else if (signalStrength < -70)
        {
            return (WiFiSignalQuality_BAD);
        }
        else if (signalStrength < -67)
        {
            return (WiFiSignalQuality_NORMAL);
        }
        else if (signalStrength < -30)
        {
            return (WiFiSignalQuality_GOOD);
        }
        else
        {
            return (WiFiSignalQuality_BEST);
        }
    }
}
