
#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_H

#include <cstdint>
#include <cstddef>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#define QUEUE_PEEK_MS_TO_TICKS_TIMEOUT 5

#define MAX_NETWORK_INTERFACE_ID_LENGTH 65
#define MAX_NETWORK_INTERFACE_NAME_LENGTH 4

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
    uint64_t totalBandwithBytesPerSecondLimit;
    uint64_t totalBytesTransfered;
    uint64_t currentBandwidthBytesPerSecond;
    uint64_t timestamp;
};

class SourceData
{
private:
    bool truncateOverflows = false;
    char networkInterfaceId[MAX_NETWORK_INTERFACE_ID_LENGTH] = {'\0'};
    char networkInterfaceName[MAX_NETWORK_INTERFACE_NAME_LENGTH] = {'\0'};
    // DOWNLOAD BANDWIDTH
    uint64_t totalNetworkDownloadBandwidthLimit = 0;
    uint64_t currentTotalNetworkDownloaded = 0;
    uint64_t previousTotalNetworkDownloaded = 0;
    uint64_t currentTotalNetworkDownloadedTimestamp = 0;
    uint64_t previousTotalNetworkDownloadedTimestamp = 0;
    uint64_t currentNetworkDownloadSpeed = 0;
    // UPLOAD BANDWIDTH
    uint64_t totalNetworkUploadBandwidthLimit = 0;
    uint64_t currentTotalNetworkUploaded = 0;
    uint64_t previousTotalNetworkUploaded = 0;
    uint64_t currentTotalNetworkUploadedTimestamp = 0;
    uint64_t previousTotalNetworkUploadedTimestamp = 0;
    uint64_t currentNetworkUploadSpeed = 0;

    QueueHandle_t cpuLoadQueue;
    QueueHandle_t usedMemoryQueue;
    QueueHandle_t cpuTemperatureQueue;
    QueueHandle_t uptimeQueue;

    QueueHandle_t networkingDownloadQueue;
    QueueHandle_t networkingUploadQueue;

public:
    SourceData(bool truncateOverflows, uint64_t totalNetworkDownloadBandwidthLimit, uint64_t totalNetworkUploadBandwidthLimit, const char *networkInterfaceId, const char *networkInterfaceName);
    ~SourceData();
    // NET IFACE
    void getNetworkInterfaceId(char *buffer, size_t buffer_count);
    void getNetworkInterfaceName(char *buffer, size_t buffer_count);

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

    // NET DOWNLOAD BANDWIDTH
    SourceDataQueueNetworkingValue getCurrentNetworkDownload(void);
    bool setCurrentNetworkDownload(uint64_t totalBytes, uint64_t limitBytes, uint64_t timestamp);

    uint64_t getNetworkDownloadBandwidthLimit(void) const;
    bool setNetworkDownloadBandwidthLimit(uint64_t bytes);
    uint64_t getNetworkDownloadSpeed(void) const;
    uint64_t getNetworkDownloadSpeedTimestamp(void) const;
    uint64_t getCurrentTotalNetworkDownloaded(void);
    bool changedNetworkDownloadSpeed(uint64_t fromTimestamp) const;
    bool setCurrentTotalNetworkDownloaded(uint64_t bytes, uint64_t timestamp);

    // NET UPLOAD BANDWIDTH
    SourceDataQueueNetworkingValue getCurrentNetworkUpload(void);
    bool setCurrentNetworkUpload(uint64_t totalBytes, uint64_t limitBytes, uint64_t timestamp);

    uint64_t getNetworkUploadBandwidthLimit(void) const;
    bool setNetworkUploadBandwidthLimit(uint64_t bytes);
    uint64_t getNetworkUploadSpeed(void) const;
    uint64_t getNetworkUploadSpeedTimestamp(void) const;
    uint64_t getCurrentTotalNetworkUploaded(void);
    bool changedNetworkUploadSpeed(uint64_t fromTimestamp) const;
    bool setCurrentTotalNetworkUploaded(uint64_t bytes, uint64_t timestamp);
};

#endif
