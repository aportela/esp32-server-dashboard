#include "LGFXScreenDashboardResumeEntityUsedMemory.hpp"
#include "../../utils/Format.hpp"

using namespace aportela::microcontroller::utils;

LGFXScreenDashboardResumeEntityUsedMemory::LGFXScreenDashboardResumeEntityUsedMemory(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, "USED MEM")
{
    if (this->parentDisplay != nullptr)
    {
        this->PrintLimits("0B", "???"); // real total memory will be changed on first refresh
        this->RefreshStrValue("0000B", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
}

LGFXScreenDashboardResumeEntityUsedMemory::~LGFXScreenDashboardResumeEntityUsedMemory()
{
}

bool LGFXScreenDashboardResumeEntityUsedMemory::Refresh(bool force)
{
    SourceDataQueueUsedMemoryValue data = this->sourceData->GetCurrentUsedMemory();
    if ((data.timestamp != 0 && data.timestamp != this->timestamp) || force)
    {
        this->timestamp = data.timestamp;
        if (this->realTotalMemory != data.totalBytes)
        {
            this->realTotalMemory = data.totalBytes;
            // redraw limits
            char maxStr[6] = {'\0'};
            Format::ParseBytesToHumanString(this->realTotalMemory, maxStr, sizeof(maxStr), false, true, false);
            this->PrintLimits("0B", maxStr);
        }
        uint8_t mapped100 = this->MapUint64ValueFrom0To100(data.usedBytes, 0, data.totalBytes);
        uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->GetGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
        this->previousMappedValue = mapped100;
        this->previousGradientcolor = currentGradientColor;
        if (data.usedBytes != this->value || force)
        {
            char strValue[sizeof(this->oldStrValue)] = {'\0'};
            Format::ParseBytesToHumanString(data.usedBytes, strValue, sizeof(strValue), true, true, false);
            strcat(strValue, "  ");
            if (strcmp(strValue, this->oldStrValue) != 0 || force)
            {
                this->RefreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
                strncpy(this->oldStrValue, strValue, sizeof(this->oldStrValue));
            }
            this->value = data.usedBytes;
        }
        this->RefreshSprite(mapped100, currentGradientColor, true);
        return (true);
    }
    else
    {
        return (false);
    }
}
