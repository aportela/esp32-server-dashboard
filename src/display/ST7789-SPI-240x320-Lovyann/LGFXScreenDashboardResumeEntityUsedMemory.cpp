#include "LGFXScreenDashboardResumeEntityUsedMemory.hpp"
#include "../../utils/Format.hpp"

LGFXScreenDashboardResumeEntityUsedMemory::LGFXScreenDashboardResumeEntityUsedMemory(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, bool enabled) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, "USED MEMORY", enabled)
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

LGFXScreenDashboardResumeEntityUsedMemory::~LGFXScreenDashboardResumeEntityUsedMemory()
{
}

bool LGFXScreenDashboardResumeEntityUsedMemory::refresh(bool force)
{
    if (this->enabled)
    {
        uint64_t currentTimestamp = this->sourceData->getCurrentUsedMemoryTimestamp();
        if (this->sourceData->changedUsedMemory(this->timestamp) || force)
        {
            uint32_t currentValue = this->sourceData->getUsedMemory();
            this->timestamp = currentTimestamp;
            if (currentValue != this->value || force)
            {
                this->value = currentValue;
                uint8_t mapped100 = this->mapUint32ValueFrom0To100(this->value, 0, this->sourceData->getTotalMemory());
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
