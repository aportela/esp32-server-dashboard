#ifndef ESP32_SERVER_DASHBOARD_SOURCE_H
#define ESP32_SERVER_DASHBOARD_SOURCE_H

#include <stdint.h>
#include "ISource.hpp"
#include "SourceData.hpp"

class Source : public ISource
{
protected:
    SourceData *currentCPULoad;
    SourceData *currentMemory;
    SourceData *currentCPUTemperature;
    SourceData *currentNetworkDownloadBandwith;
    SourceData *currentNetworkUploadBandwith;

public:
    Source();
    ~Source();
    bool changed(EntitySource entity) override;
    uint64_t getPrevious(EntitySource entity);
    uint64_t getCurrent(EntitySource entity);
    uint64_t getPreviousCPULoad(void) override;
    virtual uint64_t getCurrentCPULoad(void) = 0;
    virtual uint64_t getTotalMemory(void) = 0;
    uint64_t getPreviousUsedMemory(void) override;
    virtual uint64_t getUsedMemory(void) = 0;
    uint64_t getPreviousCPUTemperature(void) override;
    virtual uint64_t getCurrentCPUTemperature(void) = 0;
    virtual uint64_t getTotalNetworkDownloadBandwith(void) = 0;
    uint64_t getPreviousUsedNetworkDownloadBandwith(void) override;
    virtual uint64_t getUsedNetworkDownloadBandwith(void) = 0;
    virtual uint64_t getTotalNetworkUploadBandwith(void) = 0;
    uint64_t getPreviousUsedNetworkUploadBandwith(void) override;
    virtual uint64_t getUsedNetworkUploadBandwith(void) = 0;
    void refresh(void);
};

#endif
