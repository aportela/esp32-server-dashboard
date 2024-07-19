#ifndef ESP32_SERVER_DASHBOARD_SOURCE_DATA_NETWORK
#define ESP32_SERVER_DASHBOARD_SOURCE_DATA_NETWORK

#include "SourceData.hpp"

class SourceDataNetworkUsedBandwith : public SourceData
{

public:
    SourceDataNetworkUsedBandwith(uint8_t min, uint8_t max);
    ~SourceDataNetworkUsedBandwith();
};

#endif
