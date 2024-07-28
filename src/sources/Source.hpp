#ifndef ESP32_SERVER_DASHBOARD_SOURCE_H
#define ESP32_SERVER_DASHBOARD_SOURCE_H

#include <stdint.h>
#include "ISource.hpp"
#include "EntityData.hpp"
#include "../EntityType.hpp"

class Source : public ISource
{
protected:
    EntityData *currentGlobalCPULoadData;
    EntityData *currentUsedMemoryData;
    EntityData *currentGlobalCPUTemperatureData;
    EntityData *currentNetworkDownloadUsedBandwithData;
    EntityData *currentNetworkUploadUsedBandwithData;

public:
    Source();
    ~Source();
    bool changed(EntityType entity, uint64_t fromTimestamp) override;
    void setMin(EntityType entityType, uint64_t min) override;
    void setMax(EntityType entityType, uint64_t max) override;
    uint64_t getMin(EntityType entity);
    uint64_t getMax(EntityType entity);
    uint64_t getPreviousValue(EntityType entity);
    uint64_t getCurrentValue(EntityType entity);
    uint64_t getCurrentGlobalCPULoad(void);
    uint64_t getTotalMemory(void);
    uint64_t getUsedMemory(void);
    uint64_t getCurrentGlobalCPUTemperature(void);
    uint64_t getTotalNetworkDownloadBandwidth(void);
    uint64_t getCurrentNetworkDownloadUsedBandwidth(void);
    uint64_t getTotalNetworkUploadBandwidth(void);
    uint64_t getCurrentNetworkUploadUsedBandwidth(void);
};

#endif
