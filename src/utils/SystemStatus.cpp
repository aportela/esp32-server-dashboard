#include "SystemStatus.hpp"

#include <cstdio>
#include <WiFi.h>

SystemStatus::SystemStatus(void)
{
    this->startMillis = millis();
}

SystemStatus::~SystemStatus()
{
}

void SystemStatus::refresh(void)
{
    this->connected = WiFi.status() == WL_CONNECTED;
    if (this->connected)
    {
        uint8_t mac[6];
        WiFi.macAddress(mac);
        sprintf(this->macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        IPAddress ip = WiFi.localIP();
        sprintf(this->ipAddress, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        this->signalStrength = WiFi.RSSI();
        if (this->signalStrength < -90)
        {
            this->signalQuality = WIFISignalQuality_NONE;
        }
        else if (this->signalStrength < -80)
        {
            this->signalQuality = WIFISignalQuality_WORST;
        }
        else if (this->signalStrength < -70)
        {
            this->signalQuality = WIFISignalQuality_BAD;
        }
        else if (this->signalStrength < -67)
        {
            this->signalQuality = WIFISignalQuality_NORMAL;
        }
        else if (this->signalStrength < -30)
        {
            this->signalQuality = WIFISignalQuality_GOOD;
        }
        else
        {
            this->signalQuality = WIFISignalQuality_BEST;
        }
        strncpy(this->ssid, WiFi.SSID().c_str(), sizeof(this->ssid));
    }
    else
    {
        strcpy(this->macAddress, "");
        strcpy(this->ipAddress, "");
        strcpy(this->ssid, "");
        this->signalStrength = 0;
        this->signalQuality = WIFISignalQuality_NONE;
    }
}

uint64_t SystemStatus::getRuntime(void)
{
    return (millis() - this->startMillis);
}
bool SystemStatus::isWIFIConnected(void)
{
    return (this->connected);
}

void SystemStatus::getWIFIMacAddress(char *address, size_t count)
{
    strncpy(address, this->macAddress, count);
}

void SystemStatus::getWIFIIPAddress(char *address, size_t count)
{
    strncpy(address, this->ipAddress, count);
}

long SystemStatus::getWIFISignalStrength(void)
{
    return (this->signalStrength);
}

WIFISignalQuality SystemStatus::getWIFISignalQuality(void)
{
    return (this->signalQuality);
}

void SystemStatus::getWIFISSID(char *ssid, size_t count)
{
    strncpy(ssid, this->ssid, count);
}

void SystemStatus::getWIFISavedPassword(char *password, size_t count)
{
    strncpy(password, "secret", count);
}
