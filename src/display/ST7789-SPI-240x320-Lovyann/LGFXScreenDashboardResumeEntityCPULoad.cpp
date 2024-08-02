#include "LGFXScreenDashboardResumeEntityCPULoad.hpp"
#include "../../utils/Format.hpp"

LGFXScreenDashboardResumeEntityCPULoad::LGFXScreenDashboardResumeEntityCPULoad(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, bool enabled) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, "CPU LOAD", enabled)
{
    if (this->parentDisplay != nullptr)
    {
        if (this->enabled)
        {
            // this is used for init default value and printing the char "%" (on refresh only print value without char "%" to speed up things)
            this->refreshStrValue("000.00 %", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
        }
        else
        {
            // TODO: SHOW WARNING ???
        }
    }
}

LGFXScreenDashboardResumeEntityCPULoad::~LGFXScreenDashboardResumeEntityCPULoad()
{
}

bool LGFXScreenDashboardResumeEntityCPULoad::refresh(bool force)
{
    if (this->enabled)
    {
        uint64_t currentTimestamp = this->sourceData->getCurrentCPULoadTimestamp();
        if (this->sourceData->changedCPULoad(this->timestamp) || force)
        {
            float currentValue = this->sourceData->getCurrentCPULoad();
            this->timestamp = currentTimestamp;
            uint8_t mapped100 = this->mapFloatValueFrom0To100(currentValue, this->sourceData->getMinCPULoad(), this->sourceData->getMaxCPULoad());
            uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->getGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
            this->previousMappedValue = mapped100;
            this->previousGradientcolor = currentGradientColor;
            if (currentValue != this->value || force)
            {
                char strValue[8] = {'\0'};
                // 4 chars for integer part (left zero padded) + 1 char for decimal point + 2 chars for decimals
                Format::parseFloatIntoCharArray(currentValue, 2, 6, strValue, sizeof(strValue));
                this->refreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
                this->value = currentValue;
            }
            this->refreshSprite(mapped100, currentGradientColor);
            return (true);
        }
    }
    return (false);
}
