#include "LGFX.hpp"
#include "LGFXScreenInfo.hpp"
#include "LGFXScreenDashboardResume.hpp"
#include "../../sources/dummy/DummySource.hpp"
#include "../../utils/Format.hpp"
#include "../../utils/FPS.hpp"
#include "../SizesAndOffsets-320x240.hpp"

LGFX::LGFX(uint8_t PIN_SDA, uint8_t PIN_SCL, uint8_t PIN_CS, uint8_t PIN_DC, uint8_t PIN_RST, uint16_t width, uint16_t height, uint8_t rotation)
{
    auto cfg = _bus_instance.config();
    cfg.spi_host = SPI2_HOST;
    cfg.spi_mode = 0;
    cfg.freq_write = 40000000;
    cfg.freq_read = 16000000;
    cfg.spi_3wire = false;
    cfg.use_lock = true;
    cfg.dma_channel = 1;
    cfg.pin_sclk = PIN_SCL;
    cfg.pin_mosi = PIN_SDA;
    cfg.pin_miso = -1;
    cfg.pin_dc = PIN_DC;
    _bus_instance.config(cfg);
    _panel_instance.setBus(&_bus_instance);

    auto cfg_panel = _panel_instance.config();
    cfg_panel.pin_cs = PIN_CS;
    cfg_panel.pin_rst = PIN_RST;
    cfg_panel.pin_busy = -1;
    cfg_panel.memory_width = width;
    cfg_panel.memory_height = height;
    cfg_panel.panel_width = width;
    cfg_panel.panel_height = height;
    cfg_panel.offset_x = 0;
    cfg_panel.offset_y = 0;
    cfg_panel.offset_rotation = rotation;
    cfg_panel.dummy_read_pixel = 8;
    cfg_panel.dummy_read_bits = 1;
    cfg_panel.readable = true;
    cfg_panel.invert = true;
    cfg_panel.rgb_order = false;
    cfg_panel.dlen_16bit = false;
    cfg_panel.bus_shared = true;
    _panel_instance.config(cfg_panel);

    setPanel(&_panel_instance);

    this->init();
    this->fillScreen(TFT_BLACK);
}

LGFX::~LGFX()
{
    if (this->currentScreen != nullptr)
    {
        delete this->currentScreen;
        this->currentScreen = nullptr;
    }
    this->sourceData = nullptr;
}

void LGFX::SetSourceData(SourceData *src)
{
    this->sourceData = src;
}

bool LGFX::setDashboardCount(uint8_t count)
{
    if (count <= MAX_DASHBOARDS)
    {
        this->dashboardCount = count;
        return (true);
    }
    else
    {
        return (false);
    }
}

bool LGFX::SetDashboardItems(uint8_t index, const DASHBOARD_ITEM_TYPE items[DASHBOARD_ITEM_COUNT])
{
    if (index < this->dashboardCount)
    {
        for (int i = 0; i < DASHBOARD_ITEM_COUNT; i++)
        {
            this->dashboardsItems[index][i] = items[i];
        }
        return (true);
    }
    else
    {
        return (false);
    }
}

void LGFX::InitScreen(SCREEN_TYPE screenType)
{
    switch (screenType)
    {
    case SCREEN_TYPE_INFO:
        if (this->currentScreen == nullptr)
        {
            this->currentScreen = new LGFXScreenInfo(this);
        }
        this->currentScreenType = screenType;
        break;
    case SCREEN_TYPE_DASHBOARD_REALTIME_GRAPHS:
        if (this->currentScreen == nullptr)
        {
            DASHBOARD_ITEM_TYPE items[DASHBOARD_ITEM_COUNT] = {
                (DASHBOARD_ITEM_TYPE)this->dashboardsItems[this->currentDashboardIndex][0],
                (DASHBOARD_ITEM_TYPE)this->dashboardsItems[this->currentDashboardIndex][1],
                (DASHBOARD_ITEM_TYPE)this->dashboardsItems[this->currentDashboardIndex][2],
                (DASHBOARD_ITEM_TYPE)this->dashboardsItems[this->currentDashboardIndex][3],
                (DASHBOARD_ITEM_TYPE)this->dashboardsItems[this->currentDashboardIndex][4]};
            this->currentScreen = new LGFXScreenDashboardResume(this, this->sourceData, this->currentDashboardIndex, items);
        }
        this->currentScreenType = screenType;
        break;
    case SCREEN_TYPE_NONE:
    default:
        break;
    }
}

void LGFX::DeleteCurrentScreen(void)
{
    if (this->currentScreenType != SCREEN_TYPE_NONE)
    {
        if (this->currentScreen != nullptr)
        {
            delete this->currentScreen;
            this->currentScreen = nullptr;
            this->currentScreenType == SCREEN_TYPE_NONE;
        }
    }
}

SCREEN_TYPE LGFX::GetCurrentScreenType(void)
{
    return (this->currentScreenType);
}

bool LGFX::FlipToScreen(SCREEN_TYPE screenType)
{
    if (screenType != this->currentScreenType)
    {
        this->DeleteCurrentScreen();
        this->currentDashboardIndex = 0;
        this->InitScreen(screenType);
        return (true);
    }
    else if (screenType == SCREEN_TYPE_DASHBOARD_REALTIME_GRAPHS)
    {
        this->DeleteCurrentScreen();
        if (this->currentDashboardIndex < this->dashboardCount - 1)
        {
            // toggle between available dashboards
            this->currentDashboardIndex++;
            this->InitScreen(screenType);
        }
        else
        {
            // if current dashboard is last, return to info screen
            this->InitScreen(SCREEN_TYPE_INFO);
        }
        return (true);
    }
    else
    {
        return (false);
    }
}

bool LGFX::ToggleScreen(void)
{
    bool success = false;
    switch (this->GetCurrentScreenType())
    {
    case SCREEN_TYPE_INFO:
        success = this->FlipToScreen(SCREEN_TYPE_DASHBOARD_REALTIME_GRAPHS);
        break;
    case SCREEN_TYPE_DASHBOARD_REALTIME_GRAPHS:
        if (this->currentDashboardIndex < this->dashboardCount)
        {
            success = this->FlipToScreen(SCREEN_TYPE_DASHBOARD_REALTIME_GRAPHS);
        }
        else
        {
            success = this->FlipToScreen(SCREEN_TYPE_INFO);
        }
        break;
    }
    return (success);
}

bool LGFX::Refresh(void)
{
    if (this->currentScreenType != SCREEN_TYPE_NONE)
    {
        if (this->currentScreen != nullptr)
        {
            return (this->currentScreen->Refresh(false));
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}
