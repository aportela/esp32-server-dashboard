#include "LGFXScreenMemoryDetails.hpp"
#ifdef DEBUG_FPS
#include "../../utils/FPS.hpp"
#endif // DEBUG_FPS
#define METER_GRAPH_WIDTH 185
#define METER_GRAPH_HEIGHT 34
#define METER_GRAPH_BG_COLOR TFT_BLACK

// #define DEBUG_FPS // If defined, this will replace bottom hostname info with "FPS" counter (used only to "bench" & DEBUG_FPS optimizations)

LGFXScreenMemoryDetails::LGFXScreenMemoryDetails(LovyanGFX *display, SourceData *sourceData) : LGFXScreen(display)
{
    if (sourceData != nullptr)
    {
        this->currentSourceData = sourceData;
        if (display != nullptr)
        {
#ifdef DEBUG_FPS
            FPS::Init();
#endif // DEBUG_FPS
            this->memoryBlock1 = new LGFXScreenDashboardResumeEntityMemory(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 0, MEMORY_USAGE_TYPE_USED);
            this->memoryBlock2 = new LGFXScreenDashboardResumeEntityMemory(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 1, MEMORY_USAGE_TYPE_FREE);
            this->memoryBlock3 = new LGFXScreenDashboardResumeEntityMemory(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 2, MEMORY_USAGE_TYPE_AVAILABLE);
            this->memoryBlock4 = new LGFXScreenDashboardResumeEntityMemory(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 3, MEMORY_USAGE_TYPE_BUFFERED);
            this->memoryBlock5 = new LGFXScreenDashboardResumeEntityMemory(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 4, MEMORY_USAGE_TYPE_CACHED);
            this->Refresh(true);
        }
    }
}

LGFXScreenMemoryDetails::~LGFXScreenMemoryDetails()
{
    if (this->memoryBlock1 != nullptr)
    {
        delete this->memoryBlock1;
        this->memoryBlock1 = nullptr;
    }
    if (this->memoryBlock2 != nullptr)
    {
        delete this->memoryBlock2;
        this->memoryBlock2 = nullptr;
    }
    if (this->memoryBlock3 != nullptr)
    {
        delete this->memoryBlock3;
        this->memoryBlock3 = nullptr;
    }
    if (this->memoryBlock4 != nullptr)
    {
        delete this->memoryBlock4;
        this->memoryBlock4 = nullptr;
    }
    if (this->memoryBlock5 != nullptr)
    {
        delete this->memoryBlock5;
        this->memoryBlock5 = nullptr;
    }
    this->currentSourceData = nullptr;
}

bool LGFXScreenMemoryDetails::Refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        bool refreshed = false;
        if (this->currentSourceData != nullptr)
        {
            bool r1 = this->memoryBlock1->Refresh(false);
            bool r2 = this->memoryBlock2->Refresh(false);
            bool r3 = this->memoryBlock3->Refresh(false);
            bool r4 = this->memoryBlock4->Refresh(false);
            bool r5 = this->memoryBlock5->Refresh(false);
            refreshed = r1 || r2 || r3 || r4 || r5;
        }
        return (refreshed);
    }
    else
    {
        return (false);
    }
}
