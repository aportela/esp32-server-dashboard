#include "LGFXScreenDashboardResumeEntityMemory.hpp"
#include "../../utils/Format.hpp"

using namespace aportela::microcontroller::utils;

LGFXScreenDashboardResumeEntityMemory::LGFXScreenDashboardResumeEntityMemory(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, DASHBOARD_ITEM_TYPE memoryUsageType) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, DASHBOARD_ITEM_TYPE_LABEL[memoryUsageType])
{
    this->memoryUsageType = memoryUsageType;
    if (this->parentDisplay != nullptr)
    {
        this->PrintLimits("0B", "???"); // real total memory will be changed on first refresh
        this->RefreshStrValue("0000B", LGFX_SCR_DRE_FONT_COLOR, LGFX_SCR_DRE_FONT_BG_COLOR);
    }
}

LGFXScreenDashboardResumeEntityMemory::~LGFXScreenDashboardResumeEntityMemory()
{
}

bool LGFXScreenDashboardResumeEntityMemory::Refresh(bool force)
{
    SourceDataQueueUsedMemoryValues data;
    this->sourceData->GetCurrentMemoryData(data);
    if ((data.timestamp != 0 && data.timestamp != this->timestamp) || force)
    {
        this->timestamp = data.timestamp;
        uint64_t memUsageValue = 0;
        switch (this->memoryUsageType)
        {
        case DASHBOARD_ITEM_TYPE_MEM_ACTIVE:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.active;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_AVAILABLE:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.available;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_BUFFERED:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.buffered;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_CACHED:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.cached;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_DIRTY:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.dirty;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_FREE:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.free;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_INACTIVE:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.inactive;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_LAUNDRY:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.laundry;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_MAPPED:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.mapped;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_SHARED:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.shared;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_SWAP_CACHED:
            if (this->totalValue != data.swapTotal)
            {
                this->totalValue = data.swapTotal;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.swapCached;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_SWAP_FREE:
            if (this->totalValue != data.swapTotal)
            {
                this->totalValue = data.swapTotal;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.swapFree;
            break;
        case DASHBOARD_ITEM_TYPE_MEM_USED:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false, KILO_BYTE_DIVISOR_1024);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.used;
            break;
        default:
            // invalid mem type
            return (false);
            break;
        }
        memUsageValue = this->sourceData->clamp(memUsageValue, 0, data.total);
        uint8_t mapped100 = this->MapUint64ValueFrom0To100(memUsageValue, 0, data.total);
        uint16_t currentGradientColor = (mapped100 != this->previousMappedValue) ? this->GetGradientColorFrom0To100(mapped100) : this->previousGradientcolor;
        this->previousMappedValue = mapped100;
        this->previousGradientcolor = currentGradientColor;
        if (memUsageValue != this->value || force)
        {
            char strValue[sizeof(this->oldStrValue)] = {'\0'};
            Format::ParseBytesToHumanString(memUsageValue, strValue, sizeof(strValue), true, true, false);
            strcat(strValue, "  ");
            if (strcmp(strValue, this->oldStrValue) != 0 || force)
            {
                this->RefreshStrValue(strValue, currentGradientColor, LGFX_SCR_DRE_FONT_BG_COLOR);
                strncpy(this->oldStrValue, strValue, sizeof(this->oldStrValue));
            }
            this->value = memUsageValue;
        }
        this->RefreshSprite(mapped100, currentGradientColor, true);
        return (true);
    }
    else
    {
        return (false);
    }
    return (false);
}
