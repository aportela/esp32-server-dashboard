#include "LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth.hpp"
#include "../../utils/Format.hpp"
#include <cstring>
#include <cstdio>

using namespace aportela::microcontroller::utils;

LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth::LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NetBandwidthType type, LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntityNetUsedBandWidth(type, display, sourceData, width, height, xOffset, yOffset)
{
    if (this->parentDisplay != nullptr)
    {
        this->PrintLimits("0B/s", "1KB/s");
        this->RefreshStrValue("0000 B/s", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
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

bool LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth::Refresh(bool force)
{
    uint64_t currentTimestamp = 0;
    SourceDataQueueNetworkingValue networkData;
    this->sourceData->GetCurrentNetwork(networkData);
    if (networkData.timestamp != this->timestamp || force)
    {
        uint64_t diffBytes = this->type == NET_BANDWIDTH_TYPE_DOWNLOAD ? (networkData.bytesRecv - this->previousBytesRecv) : (networkData.bytesSent - this->previousBytesSent);
        float diffSeconds = (networkData.timestamp - this->timestamp) / 1000.0;
        uint64_t currentValue = diffSeconds > 0 ? diffBytes / diffSeconds : 0;
        this->previousBytesRecv = networkData.bytesRecv;
        this->previousBytesSent = networkData.bytesSent;
        this->timestamp = networkData.timestamp;
        this->dynamicScaleValuesFIFO->Push(currentValue);
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
            uint64_t maxStoredValue = this->dynamicScaleValuesFIFO->GetMaxValue();
            while (this->currentByteScale > 0 && this->byteScales[this->currentByteScale - 1] > maxStoredValue)
            {
                this->currentByteScale--;
                changeScaleRequired = true;
            }
        }
        if (changeScaleRequired)
        {
            char currentStrScale[sizeof(this->oldStrValue)] = {'\0'};
            Format::ParseBytesToHumanString(this->byteScales[this->currentByteScale], currentStrScale, sizeof(currentStrScale), false, true, true, KILO_BYTE_DIVISOR_1000);
            strcat(currentStrScale, " ");
            this->PrintLimits("0B", currentStrScale);
            this->ClearSprite();
            size_t index = this->dynamicScaleValuesFIFO->GetHead();
            for (size_t i = 0; i < this->dynamicScaleValuesFIFO->GetCount(); ++i)
            {
                uint8_t mapped100 = this->MapUint64ValueFrom0To100(this->dynamicScaleValuesFIFO->GetValueAt(index), 0, this->byteScales[this->currentByteScale]);
                uint16_t currentGradientColor = (true || mapped100 != this->previousMappedValue) ? this->GetGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
                this->previousMappedValue = mapped100;
                this->previousGradientcolor = currentGradientColor;
                // TODO: only on last index
                if (i == this->dynamicScaleValuesFIFO->GetCount() - 1 && (this->dynamicScaleValuesFIFO->GetValueAt(index) != this->value) || force)
                {
                    char strValue[sizeof(this->oldStrValue)] = {'\0'};
                    Format::ParseBytesToHumanString(this->dynamicScaleValuesFIFO->GetValueAt(index), strValue, sizeof(strValue), true, true, true, KILO_BYTE_DIVISOR_1000);
                    strcat(strValue, "  ");
                    this->RefreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
                    this->value = this->dynamicScaleValuesFIFO->GetValueAt(index);
                }
                this->RefreshSprite(mapped100, currentGradientColor, false);
                index = (index + 1) % this->dynamicScaleValuesFIFO->GetSize();
            }
            this->DumpSprite();
            return (true);
        }
        else
        {
            uint8_t mapped100 = this->MapUint64ValueFrom0To100(currentValue, 0, this->byteScales[this->currentByteScale]);
            uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->GetGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
            this->previousMappedValue = mapped100;
            this->previousGradientcolor = currentGradientColor;
            if (currentValue != this->value || force)
            {
                char strValue[sizeof(this->oldStrValue)] = {'\0'};
                Format::ParseBytesToHumanString(currentValue, strValue, sizeof(strValue), true, true, true, KILO_BYTE_DIVISOR_1000);
                strcat(strValue, "  ");
                if (strcmp(strValue, this->oldStrValue) != 0 || force)
                {
                    this->RefreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
                    strncpy(this->oldStrValue, strValue, sizeof(this->oldStrValue));
                }
                this->value = currentValue;
            }
            this->RefreshSprite(mapped100, currentGradientColor, true);
            return (true);
        }
    }
    else
    {
        return (false);
    }
}
