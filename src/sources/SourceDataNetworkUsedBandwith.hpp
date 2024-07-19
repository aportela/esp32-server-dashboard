#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_NETWORK
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_NETWORK

#include "SourceData.hpp"

class SourceDataNetworkUsedBandwith : public SourceData
{

public:
    SourceDataNetworkUsedBandwith(uint64_t min, uint64_t max);
    ~SourceDataNetworkUsedBandwith();
};

#endif
