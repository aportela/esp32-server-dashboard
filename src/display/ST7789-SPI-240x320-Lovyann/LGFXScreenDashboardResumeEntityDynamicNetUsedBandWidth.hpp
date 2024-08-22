#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_NETWORK_DYNAMIC_BANDWIDTH_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_NETWORK_DYNAMIC_BANDWIDTH_H

#include "LGFXScreenDashboardResumeEntityNetUsedBandWidth.hpp"
#include "../../utils/Uint64TFIFO.hpp"

using namespace aportela::microcontroller::utils;

class LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth : public LGFXScreenDashboardResumeEntityNetUsedBandWidth
{
private:
    char oldStrValue[24] = {'\0'};
    const uint64_t byteScales[25] = {
        1024,        // 1 Kbyte
        2048,        // 2 Kbytes
        4096,        // 4 Kbtes
        8192,        // 8 Kbytes
        16384,       // 16 Kbytes
        32768,       // 32 Kbytes
        65536,       // 64 Kbytes
        131072,      // 128 Kbytes
        262144,      // 256 Kbytes
        524288,      // 512 Kbytes
        1048576,     // 1 Mbyte
        2097152,     // 2 Mbytes
        5242880,     // 5 MBytes
        10485760,    // 10 Mbytes
        20971520,    // 25 Mbytes
        52428800,    // 50 Mbytes
        104857600,   // 100 Mbytes
        209715200,   // 200 Mbytes
        524288000,   // 500 Mbytes
        1048576000,  // 1 Gbyte
        2097152000,  // 2 Gbytes
        5242880000,  // 5 Gbytes
        10485760000, // 10 Gbytes
        26214400000, // 25 Gbytes
        52428800000, // 50 Gbytes
    };
    uint8_t currentByteScale = 0;
    Uint64TFIFO *dynamicScaleValuesFIFO = nullptr;

public:
    LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset);
    ~LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth();
    bool refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_NETWORK_DYNAMIC_BANDWIDTH_H
