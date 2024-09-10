#include "LGFXScreenCPUDetails.hpp"
#ifdef DEBUG_FPS
#include "../../utils/FPS.hpp"
#endif // DEBUG_FPS
#define METER_GRAPH_WIDTH 185
#define METER_GRAPH_HEIGHT 34
#define METER_GRAPH_BG_COLOR TFT_BLACK

// #define DEBUG_FPS // If defined, this will replace bottom hostname info with "FPS" counter (used only to "bench" & DEBUG_FPS optimizations)

LGFXScreenCPUDetails::LGFXScreenCPUDetails(LovyanGFX *display, SourceData *sourceData) : LGFXScreen(display)
{
    if (sourceData != nullptr)
    {
        this->currentSourceData = sourceData;
        if (display != nullptr)
        {
#ifdef DEBUG_FPS
            FPS::Init();
#endif // DEBUG_FPS
            this->cpuLoadBlock1 = new LGFXScreenDashboardResumeEntityCPU(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 0, CPU_USAGE_TYPE_LOAD);
            this->cpuLoadBlock2 = new LGFXScreenDashboardResumeEntityCPU(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 1, CPU_USAGE_TYPE_SYSTEM);
            this->cpuLoadBlock3 = new LGFXScreenDashboardResumeEntityCPU(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 2, CPU_USAGE_TYPE_USER);
            this->cpuLoadBlock4 = new LGFXScreenDashboardResumeEntityCPU(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 3, CPU_USAGE_TYPE_IOWAIT);
            this->cpuLoadBlock5 = new LGFXScreenDashboardResumeEntityCPU(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * 4, CPU_USAGE_TYPE_IDLE);
            this->Refresh(true);
        }
    }
}

LGFXScreenCPUDetails::~LGFXScreenCPUDetails()
{
    if (this->cpuLoadBlock1 != nullptr)
    {
        delete this->cpuLoadBlock1;
        this->cpuLoadBlock1 = nullptr;
    }
    if (this->cpuLoadBlock2 != nullptr)
    {
        delete this->cpuLoadBlock2;
        this->cpuLoadBlock2 = nullptr;
    }
    if (this->cpuLoadBlock3 != nullptr)
    {
        delete this->cpuLoadBlock3;
        this->cpuLoadBlock3 = nullptr;
    }
    if (this->cpuLoadBlock4 != nullptr)
    {
        delete this->cpuLoadBlock4;
        this->cpuLoadBlock4 = nullptr;
    }
    if (this->cpuLoadBlock5 != nullptr)
    {
        delete this->cpuLoadBlock5;
        this->cpuLoadBlock5 = nullptr;
    }
    this->currentSourceData = nullptr;
}

bool LGFXScreenCPUDetails::Refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        bool refreshed = false;
        if (this->currentSourceData != nullptr)
        {
            bool r1 = this->cpuLoadBlock1->Refresh(false);
            bool r2 = this->cpuLoadBlock2->Refresh(false);
            bool r3 = this->cpuLoadBlock3->Refresh(false);
            bool r4 = this->cpuLoadBlock4->Refresh(false);
            bool r5 = this->cpuLoadBlock5->Refresh(false);
            refreshed = r1 || r2 || r3 || r4 || r5;
        }
        return (refreshed);
    }
    else
    {
        return (false);
    }
}
