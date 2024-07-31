
#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_H
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_H

#include <cstdint>
#include <cstddef>
#include "../EntityType.hpp"

class SourceData
{
private:
    // CPU LOAD
    float currentCPULoad = 0.0f;
    uint64_t currentCPULoadTimestamp = 0;
    // MEMORY
    uint64_t totalMemoryBytes = 0;
    uint64_t usedMemoryBytes = 0;
    uint64_t currentUsedMemoryTimestamp = 0;
    // CPU TEMPERATURE
    // DOWNLOAD BANDWIDTH
    // UPLOAD BANDWIDTH
    uint8_t minMemory = 0;

public:
    SourceData(uint64_t totalMemoryBytes);
    ~SourceData();
    // CPU LOAD
    uint8_t getMinCPULoad(void) const;
    uint8_t getMaxCPULoad(void) const;
    float getCurrentCPULoad(void) const;
    void parseCurrentCPULoad(char *buffer, size_t buffer_size, uint8_t decimals = 1) const;
    uint64_t getCurrentCPULoadTimestamp(void) const;
    bool changedCPULoad(uint64_t fromTimestamp) const;
    bool setCurrentCPULoad(float value, uint64_t timestamp);
    // MEMORY

    uint64_t getTotalMemoryBytes(void) const;
    uint64_t getUsedMemoryBytes(void) const;
    bool changedUsedMemoryBytes(uint64_t fromTimestamp) const;
    bool setUsedMemoryBytes(uint64_t bytes, uint64_t timestamp);
    // CPU TEMPERATURE
    // DOWNLOAD BANDWIDTH
    // UPLOAD BANDWIDTH

    void refresh(void);
};

#endif
