#include "LGFXScreenDashboardResumeEntityNetUsedBandWidth.hpp"
#include "../../utils/Format.hpp"

using namespace aportela::microcontroller::utils;

LGFXScreenDashboardResumeEntityNetUsedBandWidth::LGFXScreenDashboardResumeEntityNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, type == NET_BANDWIDTH_TYPE_DOWNLOAD ? "NET DOWN" : "NET UPLD"), type(type)
{
    if (this->parentDisplay != nullptr)
    {
        char maxStr[8] = {'\0'};
        sourceData->GetNetworkLimits(this->networkLimits);
        Format::ParseBitsToHumanString(this->type == NET_BANDWIDTH_TYPE_DOWNLOAD ? networkLimits.byteDownloadLimit : networkLimits.byteUploadLimit, maxStr, sizeof(maxStr), false, true, true, KILO_BIT_DIVISOR_1000);
        this->PrintLimits("0b/s", maxStr);
        this->RefreshStrValue("0000 B/s", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
}

LGFXScreenDashboardResumeEntityNetUsedBandWidth::~LGFXScreenDashboardResumeEntityNetUsedBandWidth()
{
}

bool LGFXScreenDashboardResumeEntityNetUsedBandWidth::Refresh(bool force)
{
    uint64_t currentTimestamp = 0;
    SourceDataQueueNetworkingValue networkData;
    this->sourceData->GetCurrentNetwork(networkData);
    if (networkData.timestamp != this->timestamp || force)
    {
        uint64_t diffBytes = (this->type == NET_BANDWIDTH_TYPE_DOWNLOAD ? (networkData.bytesRecv - this->previousBytesRecv) : (networkData.bytesSent - this->previousBytesSent));
        float diffSeconds = (networkData.timestamp - this->timestamp) / 1000.0;
        uint64_t currentBitsValue = diffSeconds > 0 ? diffBytes * 8 / diffSeconds : 0;
        uint64_t currentBytesValue = diffSeconds > 0 ? diffBytes / diffSeconds : 0;
        this->previousBytesRecv = networkData.bytesRecv;
        this->previousBytesSent = networkData.bytesSent;
        this->timestamp = networkData.timestamp;
        uint8_t mapped100 = this->type == NET_BANDWIDTH_TYPE_DOWNLOAD ? this->MapUint64ValueFrom0To100(currentBitsValue, 0, this->networkLimits.byteDownloadLimit) : this->MapUint64ValueFrom0To100(currentBitsValue, 0, this->networkLimits.byteUploadLimit);
        uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->GetGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
        this->previousMappedValue = mapped100;
        this->previousGradientcolor = currentGradientColor;
        if (currentBitsValue != this->value || force)
        {
            char strValue[24] = {'\0'};
            Format::ParseBytesToHumanString(currentBytesValue, strValue, sizeof(strValue), true, true, true, KILO_BYTE_DIVISOR_1000);
            strcat(strValue, "  ");
            this->RefreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
            this->value = currentBitsValue;
        }
        this->RefreshSprite(mapped100, currentGradientColor, true);
        return (true);
    }
    else
    {
        return (false);
    }
}
