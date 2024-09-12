#include "LGFXScreenDashboardResume.hpp"
#include <inttypes.h>
#include <cstring>
#include "../SizesAndOffsets-320x240.hpp"
#include "../../utils/WiFiManager.hpp"
#ifdef DEBUG_FPS
#include "../../utils/FPS.hpp"
#endif // DEBUG_FPS
#include "../../utils/Format.hpp"

#define METER_GRAPH_WIDTH 185
#define METER_GRAPH_HEIGHT 34
#define METER_GRAPH_BG_COLOR TFT_BLACK

#define SCREEN_BOTTOM_COMMON_TEXTDATA_FONT &fonts::Font2
#define SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE 1
#define SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR TFT_WHITE
#define SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR TFT_BLACK
#define SCREEN_BOTTOM_COMMON_TEXTDATA_X_OFFSET 0
#define SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET 224

// #define DEBUG_FPS // If defined, this will replace bottom hostname info with "FPS" counter (used only to "bench" & DEBUG_FPS optimizations)

LGFXScreenDashboardResume::LGFXScreenDashboardResume(LovyanGFX *display, SourceData *sourceData, const uint8_t dashboardIndex, const DASHBOARD_ITEM_TYPE items[DASHBOARD_ITEM_COUNT]) : LGFXScreen(display)
{
    if (sourceData != nullptr)
    {
        this->currentSourceData = sourceData;
        this->dashboardIndex = dashboardIndex;
        if (display != nullptr)
        {
#ifdef DEBUG_FPS
            FPS::Init();
#endif // DEBUG_FPS
            Serial.println("SI");
            for (uint8_t i = 0; i < DASHBOARD_ITEM_COUNT; i++)
            {
                switch (items[i])
                {
                case DASHBOARD_ITEM_TYPE_CPU_LOAD:
                case DASHBOARD_ITEM_TYPE_CPU_USER:
                case DASHBOARD_ITEM_TYPE_CPU_SYSTEM:
                case DASHBOARD_ITEM_TYPE_CPU_IDLE:
                case DASHBOARD_ITEM_TYPE_CPU_ACTIVE:
                case DASHBOARD_ITEM_TYPE_CPU_NICE:
                case DASHBOARD_ITEM_TYPE_CPU_IOWAIT:
                case DASHBOARD_ITEM_TYPE_CPU_IRQ:
                case DASHBOARD_ITEM_TYPE_CPU_SOFT_IRQ:
                case DASHBOARD_ITEM_TYPE_CPU_STEAL:
                case DASHBOARD_ITEM_TYPE_CPU_GUEST:
                case DASHBOARD_ITEM_TYPE_CPU_GUEST_NICE:
                    this->blocks[i] = new LGFXScreenDashboardResumeEntityCPU(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * i, items[i]);
                    break;
                case DASHBOARD_ITEM_TYPE_MEM_ACTIVE:
                case DASHBOARD_ITEM_TYPE_MEM_AVAILABLE:
                case DASHBOARD_ITEM_TYPE_MEM_BUFFERED:
                case DASHBOARD_ITEM_TYPE_MEM_CACHED:
                case DASHBOARD_ITEM_TYPE_MEM_DIRTY:
                case DASHBOARD_ITEM_TYPE_MEM_FREE:
                case DASHBOARD_ITEM_TYPE_MEM_INACTIVE:
                case DASHBOARD_ITEM_TYPE_MEM_LAUNDRY:
                case DASHBOARD_ITEM_TYPE_MEM_MAPPED:
                case DASHBOARD_ITEM_TYPE_MEM_SHARED:
                case DASHBOARD_ITEM_TYPE_MEM_SWAP_CACHED:
                case DASHBOARD_ITEM_TYPE_MEM_SWAP_FREE:
                case DASHBOARD_ITEM_TYPE_MEM_USED:
                    this->blocks[i] = new LGFXScreenDashboardResumeEntityMemory(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * i, items[i]);
                    break;
                case DASHBOARD_ITEM_TYPE_CPU_TEMPERATURE:
                    this->blocks[i] = new LGFXScreenDashboardResumeEntityCPUTemperature(display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * i);
                    break;
                case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_BANDWIDTH:
                    this->blocks[i] = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NET_BANDWIDTH_TYPE_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * i);
                    break;
                case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_BANDWIDTH:
                    this->blocks[i] = new LGFXScreenDashboardResumeEntityNetUsedBandWidth(NET_BANDWIDTH_TYPE_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * i);
                    break;
                case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_DOWNLOAD_DYNAMIC_BANDWIDTH:
                    this->blocks[i] = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NET_BANDWIDTH_TYPE_DOWNLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * i);
                    break;
                case DASHBOARD_ITEM_TYPE_NETWORK_INTERFACE_UPLOAD_DYNAMIC_BANDWIDTH:
                    this->blocks[i] = new LGFXScreenDashboardResumeEntityDynamicNetUsedBandWidth(NET_BANDWIDTH_TYPE_UPLOAD, display, sourceData, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, (METER_GRAPH_HEIGHT + 11) * i);
                    break;
                default:
                    break;
                }
            }
            sourceData->GetHostname(this->hostname, sizeof(this->hostname));
            this->Refresh(true);
        }
    }
}

LGFXScreenDashboardResume::~LGFXScreenDashboardResume()
{
    for (uint8_t i = 0; i < 5; i++)
    {
        if (this->blocks[i] != nullptr)
        {
            delete this->blocks[i];
            this->blocks[i] = nullptr;
        }
    }
    this->currentSourceData = nullptr;
}

bool LGFXScreenDashboardResume::RefreshBottomCommonData(bool forceDrawAll)
{
    bool changed = forceDrawAll;
    if (forceDrawAll)
    {
        this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
        this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
        this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
        this->parentDisplay->setCursor(SCREEN_BOTTOM_COMMON_TEXTDATA_X_OFFSET, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
        this->parentDisplay->printf("[%d] ", this->dashboardIndex);
#ifdef DEBUG_FPS

        this->parentDisplay->print("FPS: ");
        this->FPSValueXOffset = this->parentDisplay->getCursorX();
        this->parentDisplay->print("000 - Uptime: ");
        this->uptimeValueXOffset = this->parentDisplay->getCursorX();
        this->parentDisplay->print("NO DATA");
#else
        this->parentDisplay->print("Host: ");
        this->FPSValueXOffset = this->parentDisplay->getCursorX();
        this->parentDisplay->printf("%s - Uptime: ", strlen(this->hostname) > 0 ? this->hostname : "[NOT SET]");
        this->uptimeValueXOffset = this->parentDisplay->getCursorX();
        this->parentDisplay->print("NO DATA");
#endif
    }
#ifdef DEBUG_FPS
    uint16_t currentFPS = FPS::GetFPS();
    if (forceDrawAll || currentFPS != this->previousFPS)
    {
        this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
        this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
        this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
        this->parentDisplay->setCursor(this->FPSValueXOffset, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
        this->parentDisplay->printf("%03u", currentFPS);
        this->previousFPS = currentFPS;
        changed = true;
    }
#else
#endif // DEBUG_FPS

    SourceDataQueueUptimeValue data;
    this->currentSourceData->GetCurrentUptime(data);
    if (data.seconds > 0)
    {
        char str[sizeof(this->previousUptimeStr)];
        Format::ParseMillisToHumanString(data.seconds * 1000, str, sizeof(this->previousUptimeStr));
        if (forceDrawAll || strcmp(str, this->previousUptimeStr) != 0)
        {
            this->parentDisplay->setFont(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT);
            this->parentDisplay->setTextSize(SCREEN_BOTTOM_COMMON_TEXTDATA_FONT_SIZE);
            this->parentDisplay->setTextColor(SCREEN_BOTTOM_COMMON_TEXTDATA_COLOR, SCREEN_BOTTOM_COMMON_TEXTDATA_BG_COLOR);
            this->parentDisplay->setCursor(uptimeValueXOffset, SCREEN_BOTTOM_COMMON_TEXTDATA_Y_OFFSET);
            this->parentDisplay->printf("%s    ", str);
            strncpy(this->previousUptimeStr, str, sizeof(this->previousUptimeStr));
            changed = true;
        }
    }
    return (changed);
}

bool LGFXScreenDashboardResume::Refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        bool refreshed = false;
        if (this->currentSourceData != nullptr)
        {
            bool errorRefresh = false;
            for (uint8_t i = 0; i < 5; i++)
            {
                if (this->blocks[i] != nullptr && !this->blocks[i]->Refresh(false))
                {
                    errorRefresh = true;
                }
            }
            refreshed = !errorRefresh;
        }
        return (this->RefreshBottomCommonData(force) || refreshed);
    }
    else
    {
        return (false);
    }
}
