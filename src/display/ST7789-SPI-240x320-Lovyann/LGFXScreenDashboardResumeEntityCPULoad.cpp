#include "LGFXScreenDashboardResumeEntityCPULoad.hpp"
#include "../../utils/Format.hpp"
#include <cstdio>

using namespace aportela::microcontroller::utils;

LGFXScreenDashboardResumeEntityCPULoad::LGFXScreenDashboardResumeEntityCPULoad(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, "CPU LOAD")
{
    if (this->parentDisplay != nullptr)
    {
        char minStr[5] = {'\0'};
        std::snprintf(minStr, sizeof(minStr), "%u%%", MIN_CPU_LOAD);
        char maxStr[5] = {'\0'};
        std::snprintf(maxStr, sizeof(maxStr), "%03u%%", MAX_CPU_LOAD);
        this->PrintLimits(minStr, maxStr);
        // this is used for init default value and printing the char "%" (on refresh only print value without char "%" to speed up things)
        this->RefreshStrValue("000.00%", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
}

LGFXScreenDashboardResumeEntityCPULoad::~LGFXScreenDashboardResumeEntityCPULoad()
{
}

bool LGFXScreenDashboardResumeEntityCPULoad::Refresh(bool force)
{
    SourceDataQueueCPULoadValue data = this->sourceData->GetCurrentCPULoad();
    if ((data.timestamp != 0 && data.timestamp != this->timestamp) || force)
    {
        this->timestamp = data.timestamp;
        uint8_t mapped100 = this->MapFloatValueFrom0To100(data.loadPercent, MIN_CPU_LOAD, MAX_CPU_LOAD);
        uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->GetGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
        this->previousMappedValue = mapped100;
        this->previousGradientcolor = currentGradientColor;
        if (data.loadPercent != this->value || force)
        {
            char strValue[7] = {'\0'};
            // 3 chars for integer part (left zero padded) + 1 char for decimal point + 2 chars for decimals
            Format::ParseFloatToString(data.loadPercent, strValue, sizeof(strValue), 2, 6);
            this->RefreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
            this->value = data.loadPercent;
        }
        this->RefreshSprite(mapped100, currentGradientColor, true);
        return (true);
    }
    else
    {
        return (false);
    }
}
