#include "LGFXScreenDashboardResumeEntityNetUsedBandWidth.hpp"
#include "../../utils/Format.hpp"

using namespace aportela::microcontroller::utils;

LGFXScreenDashboardResumeEntityNetUsedBandWidth::LGFXScreenDashboardResumeEntityNetUsedBandWidth(DASHBOARD_ITEM_TYPE type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, DASHBOARD_ITEM_TYPE_LABEL[type]), type(type)
{
    if (this->parentDisplay != nullptr)
    {
        char maxStr[8] = {'\0'};
        sourceData->GetNetworkLimits(this->networkLimits);
        switch (this->type)
        {
        case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_BANDWIDTH:
            Format::ParseBitsToHumanString(networkLimits.byteDownloadLimit, maxStr, sizeof(maxStr), false, true, true, KILO_BIT_DIVISOR_1000);
            break;
        case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_BANDWIDTH:
            Format::ParseBitsToHumanString(networkLimits.byteUploadLimit, maxStr, sizeof(maxStr), false, true, true, KILO_BIT_DIVISOR_1000);
            break;
        default:
            strncpy(maxStr, "0b/s", sizeof(maxStr));
            break;
        }

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
        uint64_t diffBytes = 0;
        switch (this->type)
        {
        case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_BANDWIDTH:
            diffBytes = networkData.bytesRecv - this->previousBytesRecv;
            break;
        case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_BANDWIDTH:
            diffBytes = networkData.bytesSent - this->previousBytesSent;
            break;
        }
        float diffSeconds = (networkData.timestamp - this->timestamp) / 1000.0;
        uint64_t currentBitsValue = diffSeconds > 0 ? diffBytes * 8 / diffSeconds : 0;
        uint64_t currentBytesValue = diffSeconds > 0 ? diffBytes / diffSeconds : 0;
        this->previousBytesRecv = networkData.bytesRecv;
        this->previousBytesSent = networkData.bytesSent;
        this->timestamp = networkData.timestamp;
        uint8_t mapped100 = 0;
        switch (this->type)
        {
        case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_BANDWIDTH:
            mapped100 = this->MapUint64ValueFrom0To100(currentBitsValue, 0, this->networkLimits.byteDownloadLimit);
            break;
        case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_BANDWIDTH:
            mapped100 = this->MapUint64ValueFrom0To100(currentBitsValue, 0, this->networkLimits.byteUploadLimit);
            break;
        }
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
