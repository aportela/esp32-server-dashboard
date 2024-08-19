
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

class SourceData
{
private:
    bool truncateOverflows = false;

    /*
    uint64_t totalNetworkDownloadBandwidthLimit = 0;
    uint64_t totalNetworkUploadBandwidthLimit = 0;
    */

    QueueHandle_t cpuLoadQueue;
    QueueHandle_t usedMemoryQueue;
    QueueHandle_t cpuTemperatureQueue;
    QueueHandle_t systemUptimeQueue;
    QueueHandle_t networkingDownloadQueue;
    QueueHandle_t networkingUploadQueue;
    QueueHandle_t networkingLimitsQueue;

public:
    SourceData(bool truncateOverflows, uint64_t totalNetworkDownloadBandwidthLimit = 0, uint64_t totalNetworkUploadBandwidthLimit = 0);
    ~SourceData();

    // CPU LOAD
    SourceDataQueueCPULoadValue getCurrentCPULoad(void);
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

    // NET DOWNLOAD BANDWIDTH
    // uint64_t getNetworkDownloadBandwidthLimit(void) const;
    // bool setNetworkDownloadBandwidthLimit(uint64_t bytes);

    SourceDataQueueNetworkingValue getCurrentNetworkDownload(void);
    bool setCurrentNetworkDownload(uint64_t totalBytes, uint64_t timestamp);

    // NET UPLOAD BANDWIDTH
    // uint64_t getNetworkUploadBandwidthLimit(void) const;
    // bool setNetworkUploadBandwidthLimit(uint64_t bytes);

    SourceDataQueueNetworkingValue getCurrentNetworkUpload(void);
    bool setCurrentNetworkUpload(uint64_t totalBytes, uint64_t timestamp);
};

#endif
