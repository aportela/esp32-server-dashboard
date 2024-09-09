#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_MEMORY_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_MEMORY_H

#include "LGFXScreenDashboardResumeEntity.hpp"

enum MEMORY_USAGE_TYPE
{
    MEMORY_USAGE_TYPE_ACTIVE = 0,
    MEMORY_USAGE_TYPE_AVAILABLE = 1,
    MEMORY_USAGE_TYPE_BUFFERED = 2,
    MEMORY_USAGE_TYPE_CACHED = 3,
    MEMORY_USAGE_TYPE_COMMIT_LIMIT = 4,
    MEMORY_USAGE_TYPE_COMMITED_AS = 5,
    MEMORY_USAGE_TYPE_DIRTY = 6,
    MEMORY_USAGE_TYPE_FREE = 7,
    MEMORY_USAGE_TYPE_HIGH_FREE = 8,
    MEMORY_USAGE_TYPE_HIGH_TOTAL = 9,
    MEMORY_USAGE_TYPE_HUGE_PAGES_FREE = 10,
    MEMORY_USAGE_TYPE_HUGE_PAGE_SIZE = 11,
    MEMORY_USAGE_TYPE_HUGE_PAGES_TOTAL = 12,
    MEMORY_USAGE_TYPE_INACTIVE = 13,
    MEMORY_USAGE_TYPE_LAUNDRY = 14,
    MEMORY_USAGE_TYPE_LOW_FREE = 15,
    MEMORY_USAGE_TYPE_LOW_TOTAL = 16,
    MEMORY_USAGE_TYPE_MAPPED = 17,
    MEMORY_USAGE_TYPE_PAGED_TABLES = 18,
    MEMORY_USAGE_TYPE_SHARED = 19,
    MEMORY_USAGE_TYPE_SLAB = 20,
    MEMORY_USAGE_TYPE_SRECLAIMABLE = 21,
    MEMORY_USAGE_TYPE_SUNRECLAIM = 22,
    MEMORY_USAGE_TYPE_SWAP_CACHED = 23,
    MEMORY_USAGE_TYPE_SWAP_FREE = 24,
    MEMORY_USAGE_TYPE_SWAP_TOTAL = 25,
    MEMORY_USAGE_TYPE_TOTAL = 26,
    MEMORY_USAGE_TYPE_USED = 27,
    MEMORY_USAGE_TYPE_VMALLOC_CHUNK = 28,
    MEMORY_USAGE_TYPE_VMALLOC_TOTAL = 29,
    MEMORY_USAGE_TYPE_VMALLOC_USED = 30,
    MEMORY_USAGE_TYPE_WIRED = 31,
    MEMORY_USAGE_TYPE_WRITEBACK = 32,
    MEMORY_USAGE_TYPE_WRITEBACK_TMP = 33,
};

class LGFXScreenDashboardResumeEntityUsedMemory : public LGFXScreenDashboardResumeEntity
{
private:
    uint64_t timestamp = 0;
    uint64_t value = 0;
    uint64_t totalValue = 0;
    char oldStrValue[24] = {'\0'};
    MEMORY_USAGE_TYPE memoryUsageType = MEMORY_USAGE_TYPE_USED;

public:
    LGFXScreenDashboardResumeEntityUsedMemory(LovyanGFX *display, SourceData *sourceData, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, MEMORY_USAGE_TYPE memoryUsageType = MEMORY_USAGE_TYPE_USED);
    ~LGFXScreenDashboardResumeEntityUsedMemory();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_DASHBOARD_RESUME_ENTITY_USED_MEMORY_H
