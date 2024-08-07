#include "LGFXScreenDashboardResumeEntityNetUsedBandWidth.hpp"
#include "../../utils/Format.hpp"

LGFXScreenDashboardResumeEntityNetUsedBandWidth::LGFXScreenDashboardResumeEntityNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, bool dynamicScale) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, type == NBT_DOWNLOAD ? "NET DOWNLOAD" : "NET UPLOAD"), type(type), dynamicScale(dynamicScale)
{
    if (this->parentDisplay != nullptr)
    {
        this->refreshStrValue("0000 Bytes/seg", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
    if (dynamicScale)
    {
        this->dynamicScaleValuesFIFO = new Uint64TFIFO(width);
        if (this->type == NBT_DOWNLOAD)
        {
            this->originalBandwidth = sourceData->getNetworkDownloadBandwidthLimit();
        }
        else
        {
            this->originalBandwidth = sourceData->getNetworkUploadBandwidthLimit();
        }
    }
}

LGFXScreenDashboardResumeEntityNetUsedBandWidth::~LGFXScreenDashboardResumeEntityNetUsedBandWidth()
{
    if (this->dynamicScaleValuesFIFO != nullptr)
    {
        delete this->dynamicScaleValuesFIFO;
        this->dynamicScaleValuesFIFO = nullptr;
    }
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

        if (this->dynamicScale)
        {
            this->dynamicScaleValuesFIFO->push(currentValue);
            bool growScaleRequired = false;
            const size_t byteScalesSize = sizeof(this->byteScales) / sizeof(this->byteScales[0]);
            while (this->byteScales[this->currentByteScale] < currentValue && this->currentByteScale < byteScalesSize)
            {
                this->currentByteScale++;
                growScaleRequired = true;
            }
            if (growScaleRequired)
            {
                if (this->type == NBT_DOWNLOAD)
                {
                    this->sourceData->setNetworkDownloadBandwidthLimit(this->byteScales[this->currentByteScale]);
                }
                else
                {
                    this->sourceData->setNetworkUploadBandwidthLimit(this->byteScales[this->currentByteScale]);
                }
                // TODO: rewrite all FIFO into sprite & dump
            }
            else
            {
                // check for required shrink scale
                bool shrinkScaleRequired = false;
                // get FIFO max value
                uint64_t maxFIFOValue = 0;
                if (this->currentByteScale > 0 && this->currentByteScale < byteScalesSize)
                {
                    while (this->byteScales[this->currentByteScale - 1] > currentValue && this->currentByteScale > 0)
                    {
                        this->currentByteScale--;
                        shrinkScaleRequired = true;
                    }
                }
                if (shrinkScaleRequired)
                {
                    if (this->type == NBT_DOWNLOAD)
                    {
                        this->sourceData->setNetworkDownloadBandwidthLimit(this->byteScales[this->currentByteScale]);
                    }
                    else
                    {
                        this->sourceData->setNetworkUploadBandwidthLimit(this->byteScales[this->currentByteScale]);
                    }
                    // TODO: rewrite all FIFO into sprite & dump
                }
                else
                {
                    // write current value (with current scale)
                }
            }
        }
        else
        {
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
                Format::bytesToHumanStr(this->value, strValue, sizeof(strValue), true);
                strcat(strValue, "  ");
                this->refreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
                this->value = currentValue;
            }
            this->refreshSprite(mapped100, currentGradientColor, true);
        }
        return (true);
    }
    else
    {
        return (false);
    }
}
