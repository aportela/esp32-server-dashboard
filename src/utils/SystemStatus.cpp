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
    }
    else
    {
        strcpy(this->macAddress, "");
        strcpy(this->ipAddress, "");
        this->signalStrength = 0;
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

void SystemStatus::getWIFISSID(char *ssid, size_t count)
{
    strncpy(ssid, "", count);
}

void SystemStatus::getWIFISavedPassword(char *password, size_t count)
{
    strncpy(password, "secret", count);
}
