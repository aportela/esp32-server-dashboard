#ifndef ESP32_SERVER_DASHBOARD_SOURCE_INTERFACE_H
#define ESP32_SERVER_DASHBOARD_SOURCE_INTERFACE_H

#include <stdint.h>
#include "EntityData.hpp"

enum EntitySource
{
    CPU_LOAD,
    MEMORY,
    CPU_TEMPERATURE,
    NETWORK_BANDWITH_DOWNLOAD,
    NETWORK_BANDWITH_UPLOAD
};

class ISource
{
public:
    virtual ~ISource() {}
    virtual bool changed(EntitySource entity, uint64_t fromTimestamp) = 0;
    virtual uint64_t getPrevious(EntitySource entity) = 0;
    virtual uint64_t getCurrent(EntitySource entity) = 0;
    virtual uint64_t getPreviousCPULoad(void) = 0;
    virtual uint64_t getCurrentCPULoad(void) = 0;
    virtual uint64_t getTotalMemory(void) = 0;
    virtual uint64_t getPreviousUsedMemory(void) = 0;
    virtual uint64_t getUsedMemory(void) = 0;
    virtual uint64_t getPreviousCPUTemperature(void) = 0;
    virtual uint64_t getCurrentCPUTemperature(void) = 0;
    virtual uint64_t getTotalNetworkDownloadBandwith(void) = 0;
    virtual uint64_t getPreviousUsedNetworkDownloadBandwith(void) = 0;
    virtual uint64_t getUsedNetworkDownloadBandwith(void) = 0;
    virtual uint64_t getTotalNetworkUploadBandwith(void) = 0;
    virtual uint64_t getPreviousUsedNetworkUploadBandwith(void) = 0;
    virtual uint64_t getUsedNetworkUploadBandwith(void) = 0;
};

#endif
