#include "LGFXScreenDashboardResumeEntityUsedMemory.hpp"
#include "../../utils/Format.hpp"

LGFXScreenDashboardResumeEntityUsedMemory::LGFXScreenDashboardResumeEntityUsedMemory(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, "USED MEMORY")
{
    if (this->parentDisplay != nullptr)
    {
        this->refreshStrValue("0000 Bytes", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
}

LGFXScreenDashboardResumeEntityUsedMemory::~LGFXScreenDashboardResumeEntityUsedMemory()
{
}

bool LGFXScreenDashboardResumeEntityUsedMemory::refresh(bool force)
{
    uint64_t currentTimestamp = this->sourceData->getCurrentUsedMemoryTimestamp();
    if (this->sourceData->changedUsedMemory(this->timestamp) || force)
    {
        uint64_t currentValue = this->sourceData->getUsedMemory();
        this->timestamp = currentTimestamp;
        uint8_t mapped100 = this->mapUint64ValueFrom0To100(currentValue, 0, this->sourceData->getTotalMemory());
        uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->getGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
        this->previousMappedValue = mapped100;
        this->previousGradientcolor = currentGradientColor;
        if (currentValue != this->value || force)
        {
            char strValue[24] = {'\0'};
            Format::bytesToHumanStr(currentValue, strValue, sizeof(strValue), false);
            strcat(strValue, "  ");
            this->refreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
            this->value = currentValue;
        }
        this->refreshSprite(mapped100, currentGradientColor);
        return (true);
    }
    else
    {
        return (false);
    }
}
