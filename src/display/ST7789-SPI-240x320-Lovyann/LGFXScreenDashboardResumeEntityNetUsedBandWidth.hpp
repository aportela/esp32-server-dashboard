#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_NETWORK_BANDWIDTH_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_NETWORK_BANDWIDTH_H

#include "LGFXScreenDashboardResumeEntity.hpp"

enum NetBandwidthType
{
    NBT_DOWNLOAD,
    NBT_UPLOAD
};

class LGFXScreenDashboardResumeEntityNetUsedBandWidth : public LGFXScreenDashboardResumeEntity
{
private:
    uint64_t timestamp = 0;
    uint64_t value = 0;
    const uint64_t byteScales[10] = {
        1024,       // 1 Kbyte
        2048,       // 2 Kbytes
        4096,       // 4 Kbtes
        8192,       // 8 Kbytes
        16384,      // 16 Kbytes
        32768,      // 32 Kbytes
        65536,      // 64 Kbytes
        131072,     // 128 Kbytes
        262144,     // 256 Kbytes
        524288,     // 512 Kbytes
        1048576,    // 1 Mbyte
        2097152,    // 2 Mbytes
        5242880,    // 5 Bytes
        10485760,   // 10 Mbytes
        20971520,   // 25 Mbytes
        52428800,   // 50 Mbytes
        104857600,  // 100 Mbytes
        209715200,  // 200 Mbytes
        524288000,  // 500 Mbytes
        1048576000, // 1 Gbyte
    };
    uint8_t currentByteScale = 0;
    NetBandwidthType type;

public:
    LGFXScreenDashboardResumeEntityNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset);
    ~LGFXScreenDashboardResumeEntityNetUsedBandWidth();
    bool refresh(bool force = false) override;
};

#endif
