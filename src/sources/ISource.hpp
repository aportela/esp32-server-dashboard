#ifndef ESP32_SERVER_DASHBOARD_SOURCE_INTERFACE
#define ESP32_SERVER_DASHBOARD_SOURCE_INTERFACE

#include <stdint.h>
#include "SourceData.hpp"

class ISource
{
public:
    virtual ~ISource() {}
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
