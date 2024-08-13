#include "LGFXScreenDashboardResumeEntityNetUsedBandWidth.hpp"
#include "../../utils/Format.hpp"

LGFXScreenDashboardResumeEntityNetUsedBandWidth::LGFXScreenDashboardResumeEntityNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, type == NBT_DOWNLOAD ? "NET DOWNLOAD" : "NET UPLOAD"), type(type)
{
    if (this->parentDisplay != nullptr)
    {
        char maxStr[7] = {'\0'};
        if (this->type == NBT_DOWNLOAD)
        {
            Format::bytesToHumanStr(sourceData->getNetworkDownloadBandwidthLimit(), maxStr, sizeof(maxStr), true, true, false);
        }
        else
        {
            Format::bytesToHumanStr(sourceData->getNetworkUploadBandwidthLimit(), maxStr, sizeof(maxStr), true, true, false);
        }
        this->printLimits("0B", maxStr);
        this->refreshStrValue("0000 Bytes/seg", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
}

LGFXScreenDashboardResumeEntityNetUsedBandWidth::~LGFXScreenDashboardResumeEntityNetUsedBandWidth()
{
}

bool LGFXScreenDashboardResumeEntityNetUsedBandWidth::refresh(bool force)
{
    uint64_t currentTimestamp = 0;
    bool changed = false;
    if (this->type == NBT_DOWNLOAD)
    {
        currentTimestamp = this->sourceData->getNetworkDownloadSpeedTimestamp();
        changed = this->sourceData->changedNetworkDownloadSpeed(this->timestamp);
    }
    else
    {
        currentTimestamp = this->sourceData->getNetworkUploadSpeedTimestamp();
        changed = this->sourceData->changedNetworkUploadSpeed(this->timestamp);
    }
    if (changed || force)
    {
        uint64_t currentValue = 0;
        if (this->type == NBT_DOWNLOAD)
        {
            currentValue = this->sourceData->getNetworkDownloadSpeed();
        }
        else
        {
            currentValue = this->sourceData->getNetworkUploadSpeed();
        }
        this->timestamp = currentTimestamp;

        uint8_t mapped100 = 0;
        if (this->type == NBT_DOWNLOAD)
        {
            mapped100 = this->mapUint64ValueFrom0To100(currentValue, 0, this->sourceData->getNetworkDownloadBandwidthLimit());
        }
        else
        {
            mapped100 = this->mapUint64ValueFrom0To100(currentValue, 0, this->sourceData->getNetworkUploadBandwidthLimit());
        }
        uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->getGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
        this->previousMappedValue = mapped100;
        this->previousGradientcolor = currentGradientColor;
        if (currentValue != this->value || force)
        {
            char strValue[24] = {'\0'};
            Format::bytesToHumanStr(this->value, strValue, sizeof(strValue), true, false, true);
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
