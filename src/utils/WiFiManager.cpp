#include "WiFiManager.hpp"
#include <cstring>

namespace aportela::microcontroller::utils
{
    WiFiManagerConnectionCallback WiFiManager::connectionCallback = nullptr;
    char WiFiManager::wiFiSSID[SSID_CHAR_ARR_LENGTH] = {'\0'};
    char WiFiManager::wiFiPassword[PASSWORD_CHAR_ARR_LENGTH] = {'\0'};
    bool WiFiManager::tryingConnection = false;
    bool WiFiManager::validConnection = false;
    bool WiFiManager::reconnect = false;
    char WiFiManager::macAddress[MAC_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};
    char WiFiManager::ipAddress[IP_ADDRESS_CHAR_ARR_LENGTH] = {'\0'};
    long WiFiManager::signalStrength = -91;
    WIFI_SIGNAL_QUALITY WiFiManager::signalQuality = WIFI_SIGNAL_QUALITY_NONE;

    void WiFiManager::OnConnectionStatusChanged(WiFiManagerConnectionCallback callback)
    {
        if (callback != nullptr)
        {
            WiFiManager::connectionCallback = callback;
        }
    }

    void WiFiManager::SetCredentials(const char *ssid, const char *password)
    {
        WiFiManager::validConnection = false;
        std::snprintf(WiFiManager::wiFiSSID, sizeof(WiFiManager::wiFiSSID), ssid);
        std::snprintf(WiFiManager::wiFiPassword, sizeof(WiFiManager::wiFiPassword), password);
    }

    void WiFiManager::Connect(bool reconnectIfLost)
    {
        WiFiManager::reconnect = reconnectIfLost;
        if (!WiFiManager::tryingConnection)
        {
            if (strlen(WiFiManager::wiFiSSID) > 0)
            {
                WiFi.mode(WIFI_STA);
                WiFi.begin(WiFiManager::wiFiSSID, WiFiManager::wiFiPassword);
                WiFiManager::tryingConnection = true;
            }
        }
    }

    void WiFiManager::Disconnect(void)
    {
        if (WiFiManager::tryingConnection || WiFi.status() == WL_CONNECTED)
        {
            WiFi.disconnect();
        }
        WiFiManager::validConnection = false;
        WiFiManager::tryingConnection = false;
        std::snprintf(WiFiManager::macAddress, sizeof(WiFiManager::macAddress), "");
        std::snprintf(WiFiManager::ipAddress, sizeof(WiFiManager::ipAddress), "");
        WiFiManager::signalStrength = -91;
        WiFiManager::signalQuality = WIFI_SIGNAL_QUALITY_NONE;
        if (WiFiManager::connectionCallback != nullptr)
        {
            WiFiManager::connectionCallback(false);
        }
    }

    bool WiFiManager::IsConnected(void)
    {
        return (WiFi.status() == WL_CONNECTED);
    }

    void WiFiManager::Loop(void)
    {
        if (WiFiManager::tryingConnection)
        {
            delay(50);
            // connection success
            if (WiFi.status() == WL_CONNECTED)
            {
                uint8_t mac[6];
                WiFi.macAddress(mac);
                std::snprintf(WiFiManager::macAddress, sizeof(WiFiManager::macAddress), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                IPAddress ip = WiFi.localIP();
                std::snprintf(WiFiManager::ipAddress, sizeof(WiFiManager::ipAddress), "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
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

    void WiFiManager::GetSSID(char *buffer, size_t bufferSize)
    {
        strncpy(buffer, WiFiManager::wiFiSSID, bufferSize);
    }

    void WiFiManager::GetMacAddress(char *buffer, size_t bufferSize)
    {
        strncpy(buffer, WiFiManager::macAddress, bufferSize);
    }

    void WiFiManager::GetIPAddress(char *buffer, size_t bufferSize)
    {
        strncpy(buffer, WiFiManager::ipAddress, bufferSize);
    }

    long WiFiManager::GetSignalStrength(void)
    {
        return (WiFi.RSSI());
    }

    WIFI_SIGNAL_QUALITY WiFiManager::ConvertToSignalQuality(long signalStrength)
    {
        if (signalStrength < -90)
        {
            return (WIFI_SIGNAL_QUALITY_NONE);
        }
        else if (signalStrength < -80)
        {
            return (WIFI_SIGNAL_QUALITY_WORST);
        }
        else if (signalStrength < -70)
        {
            return (WIFI_SIGNAL_QUALITY_BAD);
        }
        else if (signalStrength < -67)
        {
            return (WIFI_SIGNAL_QUALITY_NORMAL);
        }
        else if (signalStrength < -30)
        {
            return (WIFI_SIGNAL_QUALITY_GOOD);
        }
        else
        {
            return (WIFI_SIGNAL_QUALITY_BEST);
        }
    }
}
