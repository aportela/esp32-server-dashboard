#include "LGFXScreenDashboardResumeEntityCPUTemperature.hpp"
#include "../../utils/Format.hpp"
#include "../DashboardItemType.hpp"
#include <cstdio>

using namespace aportela::microcontroller::utils;

LGFXScreenDashboardResumeEntityCPUTemperature::LGFXScreenDashboardResumeEntityCPUTemperature(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, DASHBOARD_ITEM_TYPE_LABEL[DASHBOARD_ITEM_TYPE_CPU_TEMPERATURE])
{
    if (this->parentDisplay != nullptr)
    {
        char minStr[5] = {'\0'};
        std::snprintf(minStr, sizeof(minStr), "%dC", (int8_t)MIN_CPU_TEMPERATURE);
        char maxStr[5] = {'\0'};
        std::snprintf(maxStr, sizeof(maxStr), "%03dC", (int8_t)MAX_CPU_TEMPERATURE);
        this->PrintLimits(minStr, maxStr);
        // this is used for init default value and printing the char "C" (on refresh only print value without char "%" to speed up things)
        this->RefreshStrValue("000.00C", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
}

LGFXScreenDashboardResumeEntityCPUTemperature::~LGFXScreenDashboardResumeEntityCPUTemperature()
{
}

bool LGFXScreenDashboardResumeEntityCPUTemperature::Refresh(bool force)
{
    SourceDataQueueCPUTemperatureValue data;
    this->sourceData->GetCurrentCPUTemperature(data);
    if ((data.timestamp != 0 && data.timestamp != this->timestamp) || force)
    {
        this->timestamp = data.timestamp;
        uint8_t mapped100 = this->MapFloatValueFrom0To100(data.celsious, MIN_CPU_TEMPERATURE, MAX_CPU_TEMPERATURE);
        uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->GetGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
        this->previousMappedValue = mapped100;
        this->previousGradientcolor = currentGradientColor;
        if (data.celsious != this->value || force)
        {
            char strValue[7] = {'\0'};
            // 3 chars for integer part (left zero padded) + 1 char for decimal point + 2 chars for decimals
            Format::ParseFloatToString(data.celsious, strValue, sizeof(strValue), 2, 6);
            this->RefreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
            this->value = data.celsious;
        }
        this->RefreshSprite(mapped100, currentGradientColor, true);
        return (true);
    }
    else
    {
        return (false);
    }
}
