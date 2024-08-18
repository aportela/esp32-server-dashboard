#include "LGFXScreenDashboardResumeEntityCPULoad.hpp"
#include "../../utils/Format.hpp"

LGFXScreenDashboardResumeEntityCPULoad::LGFXScreenDashboardResumeEntityCPULoad(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, "CPU LOAD")
{
    if (this->parentDisplay != nullptr)
    {
        char minStr[5] = {'\0'};
        snprintf(minStr, sizeof(minStr), "%u%%", sourceData->getMinCPULoad());
        char maxStr[5] = {'\0'};
        snprintf(maxStr, sizeof(maxStr), "%03u%%", sourceData->getMaxCPULoad());
        this->printLimits(minStr, maxStr);
        // this is used for init default value and printing the char "%" (on refresh only print value without char "%" to speed up things)
        this->refreshStrValue("000.00%", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
}

LGFXScreenDashboardResumeEntityCPULoad::~LGFXScreenDashboardResumeEntityCPULoad()
{
}

bool LGFXScreenDashboardResumeEntityCPULoad::refresh(bool force)
{
    SourceDataQueueDecimalValue data = this->sourceData->getCurrentCPULoad();
    if ((data.timestamp != 0 && data.timestamp != this->timestamp) || force)
    {
        this->timestamp = data.timestamp;
        uint8_t mapped100 = this->mapFloatValueFrom0To100(data.value, this->sourceData->getMinCPULoad(), this->sourceData->getMaxCPULoad());
        uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->getGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
        this->previousMappedValue = mapped100;
        this->previousGradientcolor = currentGradientColor;
        if (data.value != this->value || force)
        {
            char strValue[7] = {'\0'};
            // 3 chars for integer part (left zero padded) + 1 char for decimal point + 2 chars for decimals
            Format::parseFloatIntoCharArray(data.value, 2, 6, strValue, sizeof(strValue));
            this->refreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
            this->value = data.value;
        }
        this->refreshSprite(mapped100, currentGradientColor, true);
        return (true);
    }
    else
    {
        return (false);
    }
}
