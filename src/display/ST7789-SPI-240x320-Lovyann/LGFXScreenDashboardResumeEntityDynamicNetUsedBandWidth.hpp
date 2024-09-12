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
        1024,        // 1 KByte
        2048,        // 2 KBytes
        4096,        // 4 KBtes
        8192,        // 8 KBytes
        16384,       // 16 KBytes
        32768,       // 32 KBytes
        65536,       // 64 KBytes
        131072,      // 128 KBytes
        262144,      // 256 KBytes
        524288,      // 512 KBytes
        1048576,     // 1 MByte
        2097152,     // 2 MBytes
        5242880,     // 5 MBytes
        10485760,    // 10 MBytes
        20971520,    // 25 MBytes
        52428800,    // 50 MBytes
        104857600,   // 100 MBytes
        209715200,   // 200 MBytes
        524288000,   // 500 MBytes
        1048576000,  // 1 GByte
        2097152000,  // 2 GBytes
        5242880000,  // 5 GBytes
        10485760000, // 10 GBytes
        26214400000, // 25 GBytes
        52428800000, // 50 GBytes
    };
    uint8_t currentByteScale = 0;
    Uint64TFIFO *dynamicScaleValuesFIFO = nullptr;

public:
    LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset);
    ~LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_NETWORK_DYNAMIC_BANDWIDTH_H
