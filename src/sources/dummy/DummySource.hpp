#ifndef ESP32_SERVER_DASHBOARD_DUMMY_SOURCE_H
#define ESP32_SERVER_DASHBOARD_DUMMY_SOURCE_H

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

#define MIN_MEMORY 0
#define MAX_MEMORY 32 // GB

#define MIN_CPU_TEMPERATURE 0
#define MAX_CPU_TEMPERATURE 100 // Celsious

#define MIN_NETWORK_DOWNLOAD_BANDWITH 0
#define MAX_NETWORK_DOWNLOAD_BANDWITH 512000000 // Mbytes

#define MIN_NETWORK_UPLOAD_BANDWITH 0
#define MAX_NETWORK_UPLOAD_BANDWITH 512000000 // Mbytes

#include <stdint.h>
#include "../Source.hpp"

class DummySource : public Source
{
public:
    DummySource();
    ~DummySource();
    uint64_t getCurrentCPULoad(void) override;
    uint64_t getTotalMemory(void) override;
    uint64_t getUsedMemory(void) override;
    uint64_t getCurrentCPUTemperature(void) override;
    uint64_t getTotalNetworkDownloadBandwith(void) override;
    uint64_t getUsedNetworkDownloadBandwith(void) override;
    uint64_t getTotalNetworkUploadBandwith(void) override;
    uint64_t getUsedNetworkUploadBandwith(void) override;
};

#endif
