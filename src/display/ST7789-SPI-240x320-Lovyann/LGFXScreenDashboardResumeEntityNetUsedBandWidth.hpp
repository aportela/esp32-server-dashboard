#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_NETWORK_BANDWIDTH_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_NETWORK_BANDWIDTH_H

#include "LGFXScreenDashboardResumeEntity.hpp"

enum NetBandwidthType
{
    NET_BANDWIDTH_TYPE_DOWNLOAD,
    NET_BANDWIDTH_TYPE_UPLOAD
};

class LGFXScreenDashboardResumeEntityNetUsedBandWidth : public LGFXScreenDashboardResumeEntity
{
private:
    SourceDataQueueNetworkingLimitsValue networkLimits;

protected:
    uint64_t timestamp = 0;
    uint64_t value = 0;
    uint64_t previousBytesRecv = 0;
    uint64_t previousBytesSent = 0;
    NetBandwidthType type;

public:
    LGFXScreenDashboardResumeEntityNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset);
    ~LGFXScreenDashboardResumeEntityNetUsedBandWidth();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_NETWORK_BANDWIDTH_H
