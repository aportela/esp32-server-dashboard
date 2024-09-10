#include "LGFXScreenDashboardResumeEntityUsedMemory.hpp"
#include "../../utils/Format.hpp"

using namespace aportela::microcontroller::utils;

const char *MEMORY_LABELS[]{
    "ACTIVE",
    "AVAILABLE",
    "BUFFERED",
    "CACHED",
    "COMMIT LIMIT",
    "COMMITED AS",
    "DIRTY",
    "FREE",
    "HIGH FREE",
    "HIGH TOTAL",
    "HUGE PAGES FREE",
    "HUGE PAGE SIZE",
    "HUGE PAGES TOTAL",
    "INACTIVE",
    "LAUNDRY",
    "LOW FREE",
    "LOW TOTAL",
    "MAPPED",
    "PAGED TABLES",
    "SHARED",
    "SLAB",
    "SRECLAIMABLE",
    "SUNRECLAIM",
    "SWAP CACHED",
    "SWAP FREE",
    "SWAP TOTAL",
    "TOTAL",
    "USED",
    "VMALLOC_CHUNK",
    "VMALLOC_TOTAL",
    "VMALLOC_USED",
    "WIRED",
    "WRITEBACK",
    "WRITEBACK TMP",
};

LGFXScreenDashboardResumeEntityUsedMemory::LGFXScreenDashboardResumeEntityUsedMemory(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, MEMORY_USAGE_TYPE memoryUsageType) : LGFXScreenDashboardResumeEntity(display, sourceData, width, height, xOffset, yOffset, MEMORY_LABELS[(uint8_t)memoryUsageType])
{
    this->memoryUsageType = memoryUsageType;
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
    SourceDataQueueUsedMemoryValues data;
    this->sourceData->GetCurrentMemoryData(data);
    if ((data.timestamp != 0 && data.timestamp != this->timestamp) || force)
    {
        this->timestamp = data.timestamp;
        uint64_t memUsageValue = 0;
        switch (this->memoryUsageType)
        {
        case MEMORY_USAGE_TYPE_ACTIVE:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.active;
            break;
        case MEMORY_USAGE_TYPE_AVAILABLE:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.available;
            break;
        case MEMORY_USAGE_TYPE_BUFFERED:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.buffered;
            break;
        case MEMORY_USAGE_TYPE_CACHED:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.cached;
            break;
        case MEMORY_USAGE_TYPE_USED:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.used;
            break;
        case MEMORY_USAGE_TYPE_DIRTY:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.dirty;
            break;
        case MEMORY_USAGE_TYPE_FREE:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.free;
            break;
        case MEMORY_USAGE_TYPE_INACTIVE:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.inactive;
            break;
        case MEMORY_USAGE_TYPE_LAUNDRY:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.laundry;
            break;
        case MEMORY_USAGE_TYPE_MAPPED:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.mapped;
            break;
        case MEMORY_USAGE_TYPE_SHARED:
            if (this->totalValue != data.total)
            {
                this->totalValue = data.total;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.shared;
            break;
        case MEMORY_USAGE_TYPE_SWAP_CACHED:
            if (this->totalValue != data.swapTotal)
            {
                this->totalValue = data.swapTotal;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.swapCached;
            break;
        case MEMORY_USAGE_TYPE_SWAP_FREE:
            if (this->totalValue != data.swapTotal)
            {
                this->totalValue = data.swapTotal;
                // redraw limits
                char maxStr[6] = {'\0'};
                Format::ParseBytesToHumanString(this->totalValue, maxStr, sizeof(maxStr), false, true, false);
                this->PrintLimits("0B", maxStr);
            }
            memUsageValue = data.swapFree;
            break;
        default:
            break;
        }
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
