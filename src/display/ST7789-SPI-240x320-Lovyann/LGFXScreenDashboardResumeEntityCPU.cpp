#include "LGFXScreenDashboardResumeEntityCPU.hpp"
#include "../../utils/Format.hpp"
#include <cstdio>

using namespace aportela::microcontroller::utils;

LGFXScreenDashboardResumeEntityCPU::LGFXScreenDashboardResumeEntityCPU(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, DASHBOARD_ITEM_TYPE cpuUsageType) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, DASHBOARD_ITEM_TYPE_LABEL[cpuUsageType])
{
    this->cpuUsageType = cpuUsageType;
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

LGFXScreenDashboardResumeEntityCPU::~LGFXScreenDashboardResumeEntityCPU()
{
}

bool LGFXScreenDashboardResumeEntityCPU::Refresh(bool force)
{
    SourceDataQueueCPUValues data;
    this->sourceData->GetCurrentCPUData(data);
    if ((data.timestamp != 0 && data.timestamp != this->timestamp) || force)
    {
        float cpuUsageValue = 0.0f;
        switch (this->cpuUsageType)
        {
        case DASHBOARD_ITEM_TYPE_CPU_LOAD:
            cpuUsageValue = data.usageIdle <= 100.0f ? 100.0f - data.usageIdle : 0.0f;
            break;
        case DASHBOARD_ITEM_TYPE_CPU_USER:
            cpuUsageValue = data.usageUser;
            break;
        case DASHBOARD_ITEM_TYPE_CPU_SYSTEM:
            cpuUsageValue = data.usageSystem;
            break;
        case DASHBOARD_ITEM_TYPE_CPU_IDLE:
            cpuUsageValue = data.usageIdle;
            break;
        case DASHBOARD_ITEM_TYPE_CPU_ACTIVE:
            cpuUsageValue = data.usageActive;
            break;
        case DASHBOARD_ITEM_TYPE_CPU_NICE:
            cpuUsageValue = data.usageNice;
            break;
        case DASHBOARD_ITEM_TYPE_CPU_IOWAIT:
            cpuUsageValue = data.usageIOWait;
            break;
        case DASHBOARD_ITEM_TYPE_CPU_IRQ:
            cpuUsageValue = data.usageIRQ;
            break;
        case DASHBOARD_ITEM_TYPE_CPU_SOFT_IRQ:
            cpuUsageValue = data.usageSoftIRQ;
            break;
        case DASHBOARD_ITEM_TYPE_CPU_STEAL:
            cpuUsageValue = data.usageSteal;
            break;
        case DASHBOARD_ITEM_TYPE_CPU_GUEST:
            cpuUsageValue = data.usageGuest;
            break;
        case DASHBOARD_ITEM_TYPE_CPU_GUEST_NICE:
            cpuUsageValue = data.usageGuestNice;
            break;
        default:
            // invalid cpu usage type
            return (false);
            break;
        }
        this->timestamp = data.timestamp;
        uint8_t mapped100 = this->MapFloatValueFrom0To100(cpuUsageValue, MIN_CPU_LOAD, MAX_CPU_LOAD);
        uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->GetGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
        this->previousMappedValue = mapped100;
        this->previousGradientcolor = currentGradientColor;
        if (cpuUsageValue != this->value || force)
        {
            char strValue[7] = {'\0'};
            // 3 chars for integer part (left zero padded) + 1 char for decimal point + 2 chars for decimals
            Format::ParseFloatToString(cpuUsageValue, strValue, sizeof(strValue), 2, 6);
            this->RefreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
            this->value = cpuUsageValue;
        }
        this->RefreshSprite(mapped100, currentGradientColor, true);
        return (true);
    }
    else
    {
        return (false);
    }
}
