#ifndef ESP32_SERVER_DASHBOARD_DUMMY_SOURCE
#define ESP32_SERVER_DASHBOARD_DUMMY_SOURCE

#define MIN_CPU_LOAD 0
#define MAX_CPU_LOAD 100

#define MIN_MEMORY 0
#define MAX_MEMORY 32 // GB

#define MIN_CPU_TEMPERATURE 0
#define MAX_CPU_TEMPERATURE 100 // Celsious

#define MIN_NETWORK_DOWNLOAD_BANDWITH 0
#define MAX_NETWORK_DOWNLOAD_BANDWITH 512 // Mbytes

#define MIN_NETWORK_UPLOAD_BANDWITH 0
#define MAX_NETWORK_UPLOAD_BANDWITH 512 // Mbytes

#include <stdint.h>
#include "../ISource.hpp"
#include "../SourceData.hpp"
#include "../SourceDataCPULoad.hpp"
#include "../SourceDataMemory.hpp"
#include "../SourceDataNetworkUsedBandwith.hpp"
#include "../SourceDataCPUTemperature.hpp"

class DummySource : public ISource
{
private:
    SourceDataCPULoad *currentCPULoad;
    SourceDataMemory *currentMemory;
    SourceDataCPUTemperature *currentCPUTemperature;
    SourceDataNetworkUsedBandwith *currentNetworkDownloadBandwith;
    SourceDataNetworkUsedBandwith *currentNetworkUploadBandwith;

public:
    DummySource();
    ~DummySource();
    uint8_t getCurrentCPULoad(void);
    uint64_t getTotalMemory(void);
    uint64_t getUsedMemory(void);
    uint8_t getCurrentCPUTemperature(void);
    uint64_t getTotalNetworkDownloadBandwith(void);
    uint64_t getUsedNetworkDownloadBandwith(void);
    uint64_t getTotalNetworkUploadBandwith(void);
    uint64_t getUsedNetworkUploadBandwith(void);
};

#endif
