#ifndef ESP32_SERVER_DASHBOARD_SYSTEM_STATUS_UTILS_H
#define ESP32_SERVER_DASHBOARD_SYSTEM_STATUS_UTILS_H

#include <cstdint>
#include <cstddef>

class SystemStatus
{
private:
    uint64_t startMillis;
    bool connected;
    char macAddress[19];
    char ipAddress[16];
    long signalStrength;
    char ssid[32];
    char savedPassword[64];

public:
    SystemStatus(void);
    ~SystemStatus();
    void refresh(void);
    uint64_t getRuntime(void);
    bool isWIFIConnected(void);
    void getWIFIMacAddress(char *address, size_t count);
    void getWIFIIPAddress(char *address, size_t count);
    long getWIFISignalStrength(void);
    void getWIFISSID(char *ssid, size_t count);
    void getWIFISavedPassword(char *password, size_t count);
};

#endif
