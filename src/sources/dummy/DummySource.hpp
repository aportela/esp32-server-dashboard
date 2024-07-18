#ifndef ESP32_SERVER_DASHBOARD_DUMMY_SOURCE
#define ESP32_SERVER_DASHBOARD_DUMMY_SOURCE

#include <stdint.h>
#include "../ISource.hpp"
#include "../SourceData.hpp"
#include "../SourceDataCPU.hpp"
#include "../SourceDataMemory.hpp"
#include "../SourceDataNetwork.hpp"
#include "../SourceDataTemperature.hpp"

class DummySource : public ISource
{
private:
    SourceDataCPU *currentCPU;
    SourceDataMemory *currentMemory;
    SourceDataTemperature *currentTemperature;
    SourceDataCPU getCurrentCPU(void);
    SourceDataMemory getCurrentMemory(void);
    SourceDataNetwork getCurrentNetwork(void);

public:
    DummySource();
    ~DummySource();
    SourceData getCurrent(SourceDataType entity) override;
    uint8_t getCurrentCPULoad(void);
    uint64_t getTotalMemory(void);
    uint64_t getUsedMemory(void);
    uint8_t getCurrentTemperature(void);
};

#endif
