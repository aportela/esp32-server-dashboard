
#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_H

#include <cstdint>
#include <cstddef>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#define QUEUE_PEEK_MS_TO_TICKS_TIMEOUT 5

#define MAX_NETWORK_INTERFACE_ID_LENGTH 65

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

#define MIN_CPU_TEMPERATURE 0.0f
#define MAX_CPU_TEMPERATURE 100.0f

typedef struct SourceDataQueueCPUValues
{
    float loadPercent; // value is equal to => 100 - usageIdle
    float usageSystem;
    float usageUser;
    float usageIdle;
    float usageNice;
    float usageIOWait;
    float usageIRQ;
    float usageSoftIRQ;
    float usageGuest;
    float usageGuestNice;
    float usageSteal;
    uint64_t timestamp;
};

typedef struct SourceDataQueueUsedMemoryValue
{
    uint64_t usedBytes;
    uint64_t totalBytes;
    uint64_t timestamp;
};

typedef struct SourceDataQueueCPUTemperatureValue
{
    float celsious;
    uint64_t timestamp;
};

typedef struct SourceDataQueueUptimeValue
{
    uint64_t seconds;
    uint64_t timestamp;
};

typedef struct SourceDataQueueNetworkingValue
{
    uint64_t totalBytesTransfered;
    uint64_t currentBandwidthBytesPerSecond;
    uint64_t timestamp;
};

typedef struct SourceDataQueueNetworkingLimitsValue
{
    uint64_t byteDownloadLimit;
    uint64_t byteUploadLimit;
};

#define HOSTNAME_ARR_LENGTH 17

class SourceData
{
private:
    char hostname[HOSTNAME_ARR_LENGTH] = {'\0'};
    bool truncateOverflows = false;
    bool fixedNetworkingLimits = false;

    QueueHandle_t cpuLoadQueue;
    QueueHandle_t usedMemoryQueue;
    QueueHandle_t cpuTemperatureQueue;
    QueueHandle_t systemUptimeQueue;
    QueueHandle_t networkingDownloadQueue;
    QueueHandle_t networkingUploadQueue;
    QueueHandle_t networkingLimitsQueue;

public:
    static const uint8_t MAX_HOSTNAME_LENGTH = HOSTNAME_ARR_LENGTH - 1;

    SourceData(bool truncateOverflows, uint64_t totalNetworkDownloadBandwidthLimit = 0, uint64_t totalNetworkUploadBandwidthLimit = 0);
    ~SourceData();

    bool HasFixedNetworkingLimits(void);

    bool SetHostname(const char *hostname);
    void GetHostname(char *hostname, size_t count);

    // CPU LOAD

    SourceDataQueueCPUValues GetCurrentCPUData(void);
    bool SetCurrentCPUData(float loadPercent, float usageSystem, float usageUser, float usageIdle, float usageNice, float usageIOWait, float usageIRQ, float usageSoftIRQ, float usageGuest, float usageGuestNice, float usageSteal, uint64_t timestamp);

    // MEMORY

    SourceDataQueueUsedMemoryValue GetCurrentUsedMemory(void);
    bool SetCurrentUsedMemory(uint64_t usedBytes, uint64_t totalBytes, uint64_t timestamp);

    // CPU TEMPERATURE

    SourceDataQueueCPUTemperatureValue GetCurrentCPUTemperature(void);
    bool SetCurrentCPUTemperature(float celsious, uint64_t timestamp);

    // SYSTEM

    SourceDataQueueUptimeValue GetCurrentUptime(void);
    bool SetCurrentUptime(uint64_t seconds, uint64_t timestamp);

    // NET COMMON

    SourceDataQueueNetworkingLimitsValue GetNetworkLimits(void);
    bool SetNetworkLimits(uint64_t byteDownloadLimit, uint64_t byteUploadLimit);
    SourceDataQueueNetworkingValue GetCurrentNetworkDownload(void);
    bool SetCurrentNetworkDownload(uint64_t totalBytes, uint64_t timestamp);
    SourceDataQueueNetworkingValue GetCurrentNetworkUpload(void);
    bool SetCurrentNetworkUpload(uint64_t totalBytes, uint64_t timestamp);
};

#endif // ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
