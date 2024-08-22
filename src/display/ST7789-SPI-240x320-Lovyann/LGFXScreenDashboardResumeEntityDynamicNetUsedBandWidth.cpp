#include "LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth.hpp"
#include "../../utils/Format.hpp"

using namespace aportela::microcontroller::utils;

LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth::LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntityNetUsedBandWidth(type, display, sourceData, width, height, xOffset, yOffset)
{
    if (this->parentDisplay != nullptr)
    {
        this->printLimits("0B", "0001KB");
        this->refreshStrValue("0000 B/s", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
    this->sourceData->setNetworkLimits(this->byteScales[this->currentByteScale], this->byteScales[this->currentByteScale]);
    this->dynamicScaleValuesFIFO = new Uint64TFIFO(width);
}

LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth::~LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth()
{
    if (this->dynamicScaleValuesFIFO != nullptr)
    {
        delete this->dynamicScaleValuesFIFO;
        this->dynamicScaleValuesFIFO = nullptr;
    }
}

bool LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth::refresh(bool force)
{
    uint64_t currentTimestamp = 0;
    SourceDataQueueNetworkingValue networkData = this->type == NBT_DOWNLOAD ? this->sourceData->getCurrentNetworkDownload() : this->sourceData->getCurrentNetworkUpload();
    if (networkData.timestamp != this->timestamp || force)
    {
        uint64_t currentValue = networkData.currentBandwidthBytesPerSecond;
        this->timestamp = networkData.timestamp;
        this->dynamicScaleValuesFIFO->push(currentValue);

        bool changeScaleRequired = false;
        const size_t byteScalesSize = sizeof(this->byteScales) / sizeof(this->byteScales[0]);
        // check for required grow change
        while (this->currentByteScale < byteScalesSize && this->byteScales[this->currentByteScale] < currentValue)
        {
            this->currentByteScale++;
            changeScaleRequired = true;
        }
        if (!changeScaleRequired)
        {
            // check for required shrink change
            uint64_t maxStoredValue = this->dynamicScaleValuesFIFO->getMaxValue();
            while (this->currentByteScale > 0 && this->byteScales[this->currentByteScale - 1] > maxStoredValue)
            {
                this->currentByteScale--;
                changeScaleRequired = true;
            }
        }
        if (changeScaleRequired)
        {
            char currentStrScale[sizeof(this->oldStrValue)] = {'\0'};
            Format::bytesToHumanStr(this->byteScales[this->currentByteScale], currentStrScale, sizeof(currentStrScale), true, true, false);
            strcat(currentStrScale, " ");
            this->printLimits("0B", currentStrScale);
            this->clearSprite();
            size_t index = this->dynamicScaleValuesFIFO->getHead();
            for (size_t i = 0; i < this->dynamicScaleValuesFIFO->getCount(); ++i)
            {
                uint8_t mapped100 = this->mapUint64ValueFrom0To100(this->dynamicScaleValuesFIFO->getValueAt(index), 0, this->byteScales[this->currentByteScale]);
                uint16_t currentGradientColor = (true || mapped100 != this->previousMappedValue) ? this->getGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
                this->previousMappedValue = mapped100;
                this->previousGradientcolor = currentGradientColor;
                // TODO: only on last index
                if (i == this->dynamicScaleValuesFIFO->getCount() - 1 && (this->dynamicScaleValuesFIFO->getValueAt(index) != this->value) || force)
                {
                    char strValue[sizeof(this->oldStrValue)] = {'\0'};
                    Format::bytesToHumanStr(this->dynamicScaleValuesFIFO->getValueAt(index), strValue, sizeof(strValue), true, true, true);
                    strcat(strValue, "  ");
                    this->refreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
                    this->value = this->dynamicScaleValuesFIFO->getValueAt(index);
                }
                this->refreshSprite(mapped100, currentGradientColor, false);
                index = (index + 1) % this->dynamicScaleValuesFIFO->getSize();
            }
            this->dumpSprite();
            return (true);
        }
        else
        {
            uint8_t mapped100 = this->mapUint64ValueFrom0To100(currentValue, 0, this->byteScales[this->currentByteScale]);
            uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->getGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
            this->previousMappedValue = mapped100;
            this->previousGradientcolor = currentGradientColor;
            if (currentValue != this->value || force)
            {
                char strValue[sizeof(this->oldStrValue)] = {'\0'};
                Format::bytesToHumanStr(currentValue, strValue, sizeof(strValue), true, true, true);
                strcat(strValue, "  ");
                if (strcmp(strValue, this->oldStrValue) != 0 || force)
                {
                    this->refreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
                    strncpy(this->oldStrValue, strValue, sizeof(this->oldStrValue));
                }
                this->value = currentValue;
            }
            this->refreshSprite(mapped100, currentGradientColor, true);
            return (true);
        }
    }
    else
    {
        return (false);
    }
}
