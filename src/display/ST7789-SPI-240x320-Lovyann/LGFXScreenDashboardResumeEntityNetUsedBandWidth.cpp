#include "LGFXScreenDashboardResumeEntityNetUsedBandWidth.hpp"
#include "../../utils/Format.hpp"

LGFXScreenDashboardResumeEntityNetUsedBandWidth::LGFXScreenDashboardResumeEntityNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, type == NBT_DOWNLOAD ? "NET DOWN" : "NET UPLD"), type(type)
{
    if (this->parentDisplay != nullptr)
    {
        char maxStr[8] = {'\0'};
        SourceDataQueueNetworkingLimitsValue networkLimits = sourceData->getNetworkLimits();
        if (this->type == NBT_DOWNLOAD)
        {
            Format::bytesToHumanStr(networkLimits.byteDownloadLimit, maxStr, sizeof(maxStr), false, true, false);
        }
        else
        {
            Format::bytesToHumanStr(networkLimits.byteUploadLimit, maxStr, sizeof(maxStr), false, true, false);
        }
        this->printLimits("0B", maxStr);
        this->refreshStrValue("0000 B/s", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
}

LGFXScreenDashboardResumeEntityNetUsedBandWidth::~LGFXScreenDashboardResumeEntityNetUsedBandWidth()
{
}

bool LGFXScreenDashboardResumeEntityNetUsedBandWidth::refresh(bool force)
{
    uint64_t currentTimestamp = 0;
    // bool changed = false;
    SourceDataQueueNetworkingLimitsValue networkLimitsData = this->sourceData->getNetworkLimits();
    SourceDataQueueNetworkingValue networkData;
    if (this->type == NBT_DOWNLOAD)
    {
        networkData = this->sourceData->getCurrentNetworkDownload();
        // currentTimestamp = this->sourceData->getNetworkDownloadSpeedTimestamp();
        // changed = this->sourceData->changedNetworkDownloadSpeed(this->timestamp);
    }
    else
    {
        networkData = this->sourceData->getCurrentNetworkUpload();
        // currentTimestamp = this->sourceData->getNetworkUploadSpeedTimestamp();
        // changed = this->sourceData->changedNetworkUploadSpeed(this->timestamp);
    }
    if (networkData.timestamp != this->timestamp || force)
    {
        // uint64_t currentValue = 0;
        uint64_t currentValue = networkData.currentBandwidthBytesPerSecond;
        /*
        if (this->type == NBT_DOWNLOAD)
        {
            currentValue = this->sourceData->getNetworkDownloadSpeed();
        }
        else
        {
            currentValue = this->sourceData->getNetworkUploadSpeed();
        }
        */
        // this->timestamp = currentTimestamp;
        this->timestamp = networkData.timestamp;

        uint8_t mapped100 = 0;
        if (this->type == NBT_DOWNLOAD)
        {
            mapped100 = this->mapUint64ValueFrom0To100(currentValue, 0, networkLimitsData.byteDownloadLimit);
        }
        else
        {
            mapped100 = this->mapUint64ValueFrom0To100(currentValue, 0, networkLimitsData.byteUploadLimit);
        }
        uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->getGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
        this->previousMappedValue = mapped100;
        this->previousGradientcolor = currentGradientColor;
        if (currentValue != this->value || force)
        {
            char strValue[24] = {'\0'};
            Format::bytesToHumanStr(currentValue, strValue, sizeof(strValue), true, true, true);
            strcat(strValue, "  ");
            this->refreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
            this->value = currentValue;
        }
        this->refreshSprite(mapped100, currentGradientColor, true);
        return (true);
    }
    else
    {
        return (false);
    }
}
