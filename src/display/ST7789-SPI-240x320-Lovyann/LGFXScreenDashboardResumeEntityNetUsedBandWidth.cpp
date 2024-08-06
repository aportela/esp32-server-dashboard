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
        this->FIFOPreviousValues = (uint64_t *)calloc(width, sizeof(uint64_t));
        if (this->FIFOPreviousValues == NULL)
        {
            Serial.println("Error allocating FIFO for dynamic scaling, switching to default scale");
            this->dynamicScale = false;
        }
        else
        {
            this->FIFOFront = 0;
            this->FIFORear = 0;
            this->FIFOSize = 0;
        }
    }
}

LGFXScreenDashboardResumeEntityNetUsedBandWidth::~LGFXScreenDashboardResumeEntityNetUsedBandWidth()
{
    if (this->FIFOPreviousValues != nullptr)
    {
        free(this->FIFOPreviousValues);
        this->FIFOPreviousValues = nullptr;
    }
}

bool LGFXScreenDashboardResumeEntityNetUsedBandWidth::enqueueFIFO(uint64_t value)
{
    if (this->FIFOSize == this->width)
    {
        this->FIFOFront = (this->FIFOFront + 1) % this->width;
    }
    else
    {
        this->FIFOSize++;
    }
    this->FIFOPreviousValues[this->FIFORear] = value;
    this->FIFORear = (this->FIFORear + 1) % this->width;
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
        bool growScaleRequired = false;
        if (this->dynamicScale)
        {
            while (this->byteScales[this->currentByteScale] < currentValue && this->currentByteScale < sizeof(this->byteScales))
            {
                this->currentByteScale++;
                growScaleRequired = true;
            }
            this->enqueueFIFO(currentValue);
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
            }
        }

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
        if (this->dynamicScale)
        {
            if (!growScaleRequired)
            {
                this->refreshSprite(mapped100, currentGradientColor);
                // TODO: shrink required ?
            }
            else
            {
                // TODO: refresh ALL
            }
        }
        else
        {
            this->refreshSprite(mapped100, currentGradientColor);
        }
        return (true);
    }
    else
    {
        return (false);
    }
}
