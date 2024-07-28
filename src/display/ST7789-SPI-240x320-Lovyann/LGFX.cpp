#include "LGFX.hpp"
#include "../../sources/dummy/DummySource.hpp"
#include "src/utils/Format.hpp"
#include "../../utils/FPS.hpp"
#include <WiFi.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define GRAPH_SPRITE_WIDTH 200
#define GRAPH_SPRITE_HEIGHT 30
#define GRAPH_SPRITE_BACKGROUND TFT_BLACK

#define X_AXIS_LENGTH GRAPH_SPRITE_WIDTH + 4
#define Y_AXIS_LENGTH GRAPH_SPRITE_HEIGHT + 4

#define AXIS_COLOR TFT_WHITE

#define GRAPH_LABEL_FONT_SIZE 1

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
}

void LGFX::setSource(ISource *src)
{
    this->source = src;
}

void LGFX::initScreen(ScreenType screenType)
{
    switch (screenType)
    {
    case ST_INFO:
        if (this->currentScreen == nullptr)
        {
            this->currentScreen = new LGFXScreenInfo(this);
        }
        this->currentScreenType = screenType;
        break;
    case ST_DATA_RESUME:
        if (this->currentScreen == nullptr)
        {
            this->currentScreen = new LGFXScreenDashboardResume(this, this->source);
        }
        this->currentScreenType = screenType;
        break;
    case ST_NONE:
    default:
        break;
    }
}

void LGFX::deleteCurrentScreen(void)
{
    if (this->currentScreenType != ST_NONE)
    {
        if (this->currentScreen != nullptr)
        {
            delete this->currentScreen;
            this->currentScreen = nullptr;
            this->currentScreenType == ST_NONE;
        }
    }
}

void LGFX::flipToScreen(ScreenType screenType)
{
    if (screenType != this->currentScreenType)
    {
        this->deleteCurrentScreen();
        this->initScreen(screenType);
    }
}

void LGFX::refresh(void)
{
    if (this->currentScreenType != ST_NONE)
    {
        if (this->currentScreen != nullptr)
        {
            this->currentScreen->refresh(false);
        }
    }
}