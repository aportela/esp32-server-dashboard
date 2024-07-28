#ifndef ESP32_SERVER_DASHBOARD_SOURCE_H
#define ESP32_SERVER_DASHBOARD_SOURCE_H

#include <stdint.h>
#include "ISource.hpp"
#include "EntityData.hpp"
#include "../EntityType.hpp"

class Source : public ISource
{
protected:
    EntityData *currentGlobalCPULoadData = nullptr;
    EntityData *currentUsedMemoryData = nullptr;
    EntityData *currentGlobalCPUTemperatureData = nullptr;
    EntityData *currentNetworkDownloadUsedBandwithData = nullptr;
    EntityData *currentNetworkUploadUsedBandwithData = nullptr;

public:
    Source();
    virtual ~Source();
    bool changed(EntityType entity, uint64_t fromTimestamp) const override;
    void setMin(EntityType entityType, uint64_t min) override;
    void setMax(EntityType entityType, uint64_t max) override;
    uint64_t getMin(EntityType entity) const override;
    uint64_t getMax(EntityType entity) const override;
    uint64_t getCurrentTimestamp(EntityType entity) const override;
    uint64_t getPreviousValue(EntityType entity) const override;
    uint64_t getCurrentValue(EntityType entity) const override;
    uint64_t getCurrentGlobalCPULoad(void) const;
    uint64_t getTotalMemory(void) const;
    uint64_t getUsedMemory(void) const;
    uint64_t getCurrentGlobalCPUTemperature(void) const;
    uint64_t getTotalNetworkDownloadBandwidth(void) const;
    uint64_t getCurrentNetworkDownloadUsedBandwidth(void) const;
    uint64_t getTotalNetworkUploadBandwidth(void) const;
    uint64_t getCurrentNetworkUploadUsedBandwidth(void) const;
};

#endif
