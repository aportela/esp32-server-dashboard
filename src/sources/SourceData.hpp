
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

typedef struct SourceDataQueueCPULoadValue
{
    float loadPercent;
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

    bool setHostname(const char *hostname);
    void getHostname(char *hostname, size_t count);

    // CPU LOAD

    SourceDataQueueCPULoadValue
    getCurrentCPULoad(void);
    bool setCurrentCPULoad(float loadPercent, uint64_t timestamp);

    // MEMORY

    SourceDataQueueUsedMemoryValue getCurrentUsedMemory(void);
    bool setCurrentUsedMemory(uint64_t usedBytes, uint64_t totalBytes, uint64_t timestamp);

    // CPU TEMPERATURE

    SourceDataQueueCPUTemperatureValue getCurrentCPUTemperature(void);
    bool setCurrentCPUTemperature(float celsious, uint64_t timestamp);

    // SYSTEM

    SourceDataQueueUptimeValue getCurrentUptime(void);
    bool setCurrentUptime(uint64_t seconds, uint64_t timestamp);

    // NET COMMON

    SourceDataQueueNetworkingLimitsValue getNetworkLimits(void);
    bool setNetworkLimits(uint64_t byteDownloadLimit, uint64_t byteUploadLimit);

    SourceDataQueueNetworkingValue getCurrentNetworkDownload(void);
    bool setCurrentNetworkDownload(uint64_t totalBytes, uint64_t timestamp);

    SourceDataQueueNetworkingValue getCurrentNetworkUpload(void);
    bool setCurrentNetworkUpload(uint64_t totalBytes, uint64_t timestamp);
};

#endif // ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
