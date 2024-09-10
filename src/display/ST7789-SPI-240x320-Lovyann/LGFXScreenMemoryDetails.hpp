#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_MEMORY_DETAILS_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_MEMORY_DETAILS_H

#include "LGFXScreen.hpp"
#include "LGFXScreenDashboardResumeEntityUsedMemory.hpp"
#include "../../sources/SourceData.hpp"

class LGFXScreenMemoryDetails : public LGFXScreen
{
private:
    LGFXScreenDashboardResumeEntityUsedMemory *memoryBlock1 = nullptr;
    LGFXScreenDashboardResumeEntityUsedMemory *memoryBlock2 = nullptr;
    LGFXScreenDashboardResumeEntityUsedMemory *memoryBlock3 = nullptr;
    LGFXScreenDashboardResumeEntityUsedMemory *memoryBlock4 = nullptr;
    LGFXScreenDashboardResumeEntityUsedMemory *memoryBlock5 = nullptr;

    SourceData *currentSourceData;

public:
    LGFXScreenMemoryDetails(LovyanGFX *display, SourceData *sourceData);
    ~LGFXScreenMemoryDetails();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_MEMORY_DETAILS_H
