#include "LGFXScreenDashboardResumeEntityNetUsedBandWidth.hpp"
#include "../../utils/Format.hpp"

LGFXScreenDashboardResumeEntityNetUsedBandWidth::LGFXScreenDashboardResumeEntityNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, bool enabled) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, type == NBT_DOWNLOAD ? "WAN DOWNLOAD" : "WAN UPLOAD", enabled), type(type)
{
    if (this->parentDisplay != nullptr)
    {
        if (this->enabled)
        {
            // this is used for init default value and printing the char "%" (on refresh only print value without char "%" to speed up things)
            this->refreshStrValue("0000 B ", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
        }
        else
        {
            // TODO: SHOW WARNING ???
        }
    }
}

LGFXScreenDashboardResumeEntityNetUsedBandWidth::~LGFXScreenDashboardResumeEntityNetUsedBandWidth()
{
}

bool LGFXScreenDashboardResumeEntityNetUsedBandWidth::refresh(bool force)
{
    if (this->enabled)
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
            uint32_t currentValue = 0;
            if (this->type == NBT_DOWNLOAD)
            {
                currentValue = this->sourceData->getNetworkDownloadSpeed();
            }
            else
            {
                currentValue = this->sourceData->getNetworkUploadSpeed();
            }
            this->timestamp = currentTimestamp;
            if (currentValue != this->value || force)
            {
                this->value = currentValue;
                uint8_t mapped100 = 0;
                if (this->type == NBT_DOWNLOAD)
                {
                    mapped100 = this->mapUint32ValueFrom0To100(this->value, 0, this->sourceData->getNetworkDownloadBandwidthLimit());
                }
                else
                {
                    mapped100 = this->mapUint32ValueFrom0To100(this->value, 0, this->sourceData->getNetworkUploadBandwidthLimit());
                }
                uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->getGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
                this->previousGradientcolor = currentGradientColor;
                this->refreshSprite(mapped100, currentGradientColor);
                char strValue[16] = {'\0'};
                Format::bytesToHumanStr(this->value, strValue, sizeof(strValue));
                this->refreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
                return (true);
            }
        }
    }
    return (false);
}
