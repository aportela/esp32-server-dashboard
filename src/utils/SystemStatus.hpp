#ifndef ESP32_SERVER_DASHBOARD_SYSTEM_STATUS_UTILS_H
#define ESP32_SERVER_DASHBOARD_SYSTEM_STATUS_UTILS_H

#include <cstdint>
#include <cstddef>

enum WIFISignalQuality
{
    WIFISignalQuality_NONE,
    WIFISignalQuality_WORST,
    WIFISignalQuality_BAD,
    WIFISignalQuality_NORMAL,
    WIFISignalQuality_GOOD,
    WIFISignalQuality_BEST

};

class SystemStatus
{
private:
    uint64_t startMillis;
    bool connected;
    char macAddress[19];
    char ipAddress[16];
    long signalStrength;
    WIFISignalQuality signalQuality;
    char ssid[33];
    char savedPassword[65];

public:
    SystemStatus(void);
    ~SystemStatus();
    void refresh(void);
    uint64_t getRuntime(void);
    bool isWIFIConnected(void);
    void getWIFIMacAddress(char *address, size_t count);
    void getWIFIIPAddress(char *address, size_t count);
    long getWIFISignalStrength(void);
    WIFISignalQuality getWIFISignalQuality(void);
    void getWIFISSID(char *ssid, size_t count);
    void getWIFISavedPassword(char *password, size_t count);
};

#endif
