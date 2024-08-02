#include "LGFXScreenDashboardResumeEntityCPUTemperature.hpp"
#include "../../utils/Format.hpp"

LGFXScreenDashboardResumeEntityCPUTemperature::LGFXScreenDashboardResumeEntityCPUTemperature(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, bool enabled) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, "CPU TEMP", enabled)
{
    if (this->parentDisplay != nullptr)
    {
        if (this->enabled)
        {
            // this is used for init default value and printing the char "%" (on refresh only print value without char "%" to speed up things)
            this->refreshStrValue("000.00 C", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
        }
        else
        {
            // TODO: SHOW WARNING ???
        }
    }
}

LGFXScreenDashboardResumeEntityCPUTemperature::~LGFXScreenDashboardResumeEntityCPUTemperature()
{
}

bool LGFXScreenDashboardResumeEntityCPUTemperature::refresh(bool force)
{
    if (this->enabled)
    {
        uint64_t currentTimestamp = this->sourceData->getCurrentCPUTemperatureTimestamp();
        if (this->sourceData->changedCPUTemperature(this->timestamp) || force)
        {
            float currentValue = this->sourceData->getCurrentCPUTemperature();
            this->timestamp = currentTimestamp;
            if (currentValue != this->value || force)
            {
                this->value = currentValue;
                uint8_t mapped100 = this->mapFloatValueFrom0To100(this->value, this->sourceData->getMinCPUTemperature(), this->sourceData->getMaxCPUTemperature());
                uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->getGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
                this->previousGradientcolor = currentGradientColor;
                this->refreshSprite(mapped100, currentGradientColor);
                char strValue[8] = {'\0'};
                // 4 chars for integer part (left zero padded) + 1 char for decimal point + 2 chars for decimals
                Format::parseFloatIntoCharArray(this->value, 2, 6, strValue, sizeof(strValue));
                this->refreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
                return (true);
            }
        }
    }
    return (false);
}
