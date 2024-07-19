#ifndef ESP32_SERVER_DASHBOARD_SOURCE
#define ESP32_SERVER_DASHBOARD_SOURCE

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
    virtual uint64_t getCurrentCPULoad(void) = 0;
    virtual uint64_t getTotalMemory(void) = 0;
    virtual uint64_t getUsedMemory(void) = 0;
    virtual uint64_t getCurrentCPUTemperature(void) = 0;
    virtual uint64_t getTotalNetworkDownloadBandwith(void) = 0;
    virtual uint64_t getUsedNetworkDownloadBandwith(void) = 0;
    virtual uint64_t getTotalNetworkUploadBandwith(void) = 0;
    virtual uint64_t getUsedNetworkUploadBandwith(void) = 0;
};

#endif
