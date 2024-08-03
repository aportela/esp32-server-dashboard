
#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_H

#include <cstdint>
#include <cstddef>

class SourceData
{
private:
    // CPU LOAD
    float currentCPULoad = 0.0f;
    uint64_t currentCPULoadTimestamp = 0;
    // MEMORY
    uint64_t totalMemory = 0;
    uint64_t usedMemory = 0;
    uint64_t currentUsedMemoryTimestamp = 0;
    // CPU TEMPERATURE
    float minCPUTemperature = 0.0f;
    float maxCPUTemperature = 0.0f;
    float currentCPUTemperature = 0.0f;
    uint64_t currentCPUTemperatureTimestamp = 0;
    // SYSTEM
    uint64_t currentUptimeSeconds = 0;
    uint64_t currentUptimeSecondsTimestamp = 0;
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

public:
    SourceData(uint64_t totalMemory, float minCPUTemperature, float maxCPUTemperature, uint64_t totalNetworkDownloadBandwidthLimit, uint64_t totalNetworkUploadBandwidthLimit);
    ~SourceData();
    // CPU LOAD
    uint8_t getMinCPULoad(void) const;
    uint8_t getMaxCPULoad(void) const;
    float getCurrentCPULoad(void) const;
    uint64_t getCurrentCPULoadTimestamp(void) const;
    bool changedCPULoad(uint64_t fromTimestamp) const;
    bool setCurrentCPULoad(float value, uint64_t timestamp);
    // MEMORY
    uint64_t getTotalMemory(void) const;
    uint64_t getUsedMemory(void) const;
    uint64_t getCurrentUsedMemoryTimestamp(void) const;
    bool changedUsedMemory(uint64_t fromTimestamp) const;
    bool setUsedMemory(uint64_t bytes, uint64_t timestamp);
    // CPU TEMPERATURE
    float getMinCPUTemperature(void) const;
    bool setMinCPUTemperature(float celsious);
    float getMaxCPUTemperature(void) const;
    bool setMaxCPUTemperature(float celsious);
    float getCurrentCPUTemperature(void) const;
    uint64_t getCurrentCPUTemperatureTimestamp(void) const;
    bool changedCPUTemperature(uint64_t fromTimestamp) const;
    bool setCurrentCPUTemperature(float celsious, uint64_t timestamp);
    // SYSTEM
    uint64_t getCurrentUptimeSeconds(void) const;
    uint64_t getCurrentUptimeSecondsTimestamp(void) const;
    bool changedUptimeSeconds(uint64_t fromTimestamp) const;
    bool setCurrentUptimeSeconds(uint64_t seconds, uint64_t timestamp);
    // NET DOWNLOAD BANDWIDTH
    uint64_t getNetworkDownloadBandwidthLimit(void) const;
    bool setNetworkDownloadBandwidthLimit(uint64_t bytes);
    uint64_t getNetworkDownloadSpeed(void) const;
    uint64_t getNetworkDownloadSpeedTimestamp(void) const;
    uint64_t getCurrentTotalNetworkDownloaded(void);
    bool changedNetworkDownloadSpeed(uint64_t fromTimestamp) const;
    bool setCurrentTotalNetworkDownloaded(uint64_t bytes, uint64_t timestamp);
    // NET UPLOAD BANDWIDTH
    uint64_t getNetworkUploadBandwidthLimit(void) const;
    bool setNetworkUploadBandwidthLimit(uint64_t bytes);
    uint64_t getNetworkUploadSpeed(void) const;
    uint64_t getNetworkUploadSpeedTimestamp(void) const;
    uint64_t getCurrentTotalNetworkUploaded(void);
    bool changedNetworkUploadSpeed(uint64_t fromTimestamp) const;
    bool setCurrentTotalNetworkUploaded(uint64_t bytes, uint64_t timestamp);
};

#endif
