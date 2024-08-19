#include "LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth.hpp"
#include "../../utils/Format.hpp"

LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth::LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntityNetUsedBandWidth(type, display, sourceData, width, height, xOffset, yOffset)
{
    if (this->parentDisplay != nullptr)
    {
        this->printLimits("0B", "???");
        this->refreshStrValue("0000 B/s", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
    this->sourceData->setNetworkLimits(this->byteScales[this->currentByteScale], this->byteScales[this->currentByteScale]);
    // this->sourceData->setNetworkDownloadBandwidthLimit(this->byteScales[this->currentByteScale]);
    // this->sourceData->setNetworkUploadBandwidthLimit(this->byteScales[this->currentByteScale]);
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
    // bool changed = false;
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
            if (this->type == NBT_DOWNLOAD)
            {
                // this->sourceData->setNetworkDownloadBandwidthLimit(this->byteScales[this->currentByteScale]);
                // this->sourceData->setCurrentNetworkDownload(networkData.totalBytesTransfered, this->byteScales[this->currentByteScale], networkData.timestamp);
            }
            else
            {
                // this->sourceData->setNetworkUploadBandwidthLimit(this->byteScales[this->currentByteScale]);
                // this->sourceData->setCurrentNetworkUpload(networkData.totalBytesTransfered, this->byteScales[this->currentByteScale], networkData.timestamp);
            }
            char currentStrScale[sizeof(this->oldStrValue)] = {'\0'};
            Format::bytesToHumanStr(this->byteScales[this->currentByteScale], currentStrScale, sizeof(currentStrScale), true, true, false);
            strcat(currentStrScale, " ");
            this->printLimits("0B", currentStrScale);
            this->clearSprite();
            size_t index = this->dynamicScaleValuesFIFO->getHead();
            for (size_t i = 0; i < this->dynamicScaleValuesFIFO->getCount(); ++i)
            {
                /*
                uint8_t mapped100 = 0;
                if (this->type == NBT_DOWNLOAD)
                {
                    mapped100 = this->mapUint64ValueFrom0To100(this->dynamicScaleValuesFIFO->getValueAt(index), 0, this->byteScales[this->currentByteScale]);
                }
                else
                {
                    mapped100 = this->mapUint64ValueFrom0To100(this->dynamicScaleValuesFIFO->getValueAt(index), 0, this->byteScales[this->currentByteScale]);
                }
                */
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
            /*
            uint8_t mapped100 = 0;
            if (this->type == NBT_DOWNLOAD)
            {
                mapped100 = this->mapUint64ValueFrom0To100(currentValue, 0, this->sourceData->getNetworkDownloadBandwidthLimit());
            }
            else
            {
                mapped100 = this->mapUint64ValueFrom0To100(currentValue, 0, this->sourceData->getNetworkUploadBandwidthLimit());
            }
            */
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
