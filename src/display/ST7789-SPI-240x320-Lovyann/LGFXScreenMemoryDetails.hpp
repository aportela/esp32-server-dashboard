#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_MEMORY_DETAILS_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_MEMORY_DETAILS_H

#include "LGFXScreen.hpp"
#include "LGFXScreenDashboardResumeEntityMemory.hpp"
#include "../../sources/SourceData.hpp"

class LGFXScreenMemoryDetails : public LGFXScreen
{
private:
    LGFXScreenDashboardResumeEntityMemory *memoryBlock1 = nullptr;
    LGFXScreenDashboardResumeEntityMemory *memoryBlock2 = nullptr;
    LGFXScreenDashboardResumeEntityMemory *memoryBlock3 = nullptr;
    LGFXScreenDashboardResumeEntityMemory *memoryBlock4 = nullptr;
    LGFXScreenDashboardResumeEntityMemory *memoryBlock5 = nullptr;

    SourceData *currentSourceData;

public:
    LGFXScreenMemoryDetails(LovyanGFX *display, SourceData *sourceData);
    ~LGFXScreenMemoryDetails();
    bool Refresh(bool force = false) override;
};

#endif // ESP32_SERVER_DASHBOARD_LGFX_SCREEN_MEMORY_DETAILS_H
