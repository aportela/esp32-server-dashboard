#include "LGFX.hpp"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define X_AXIS_LENGTH 200
#define Y_AXIS_LENGTH 50

#define AXIS_COLOR TFT_WHITE

LGFX::LGFX(uint8_t SDA, uint8_t SCL, uint8_t CS, uint8_t DC, uint8_t RST)
{
    auto cfg = _bus_instance.config();
    cfg.spi_host = SPI2_HOST;
    cfg.spi_mode = 0;
    cfg.freq_write = 40000000;
    cfg.freq_read = 16000000;
    cfg.spi_3wire = false;
    cfg.use_lock = true;
    cfg.dma_channel = 1;
    cfg.pin_sclk = SCL;
    cfg.pin_mosi = SDA;
    cfg.pin_miso = -1;
    cfg.pin_dc = DC;
    _bus_instance.config(cfg);
    _panel_instance.setBus(&_bus_instance);

    auto cfg_panel = _panel_instance.config();
    cfg_panel.pin_cs = CS;
    cfg_panel.pin_rst = RST;
    cfg_panel.pin_busy = -1;
    cfg_panel.memory_width = 240;
    cfg_panel.memory_height = 320;
    cfg_panel.panel_width = 240;
    cfg_panel.panel_height = 320;
    cfg_panel.offset_x = 0;
    cfg_panel.offset_y = 0;
    cfg_panel.offset_rotation = 3;
    cfg_panel.dummy_read_pixel = 8;
    cfg_panel.dummy_read_bits = 1;
    cfg_panel.readable = true;
    cfg_panel.invert = true;
    cfg_panel.rgb_order = false;
    cfg_panel.dlen_16bit = false;
    cfg_panel.bus_shared = true;
    _panel_instance.config(cfg_panel);

    setPanel(&_panel_instance);

    this->temperatureSprite = new lgfx::LGFX_Sprite(this);
    this->temperatureSprite->createSprite(200, 100);
    this->temperatureSprite->fillSprite(TFT_BLACK);
}

LGFX::~LGFX()
{
    delete this->temperatureSprite;
    this->temperatureSprite = nullptr;
}

uint32_t LGFX::getGradientColor(int8_t value)
{
    value = constrain(value, 0, 100);

    // blue -> green
    if (value <= 33)
    {
        int blue = 255;
        int green = map(value, 0, 33, 0, 255);
        return this->color565(0, green, blue);
    }
    // green -> yellow
    else if (value <= 66)
    {
        int green = 255;
        int red = map(value, 34, 66, 0, 255);
        return this->color565(red, green, 0);
    }
    // yellow -> red
    else
    {
        int red = 255;
        int green = map(value, 67, 100, 255, 0);
        return this->color565(red, green, 0);
    }
}

void LGFX::initCPUMeter(uint8_t xOffset, uint8_t yOffset)
{
}

void LGFX::initMemoryMeter(uint8_t xOffset, uint8_t yOffset)
{
}

void LGFX::initNetworkMeter(uint8_t xOffset, uint8_t yOffset)
{
}

void LGFX::initTemperatureMeter(uint8_t xOffset, uint8_t yOffset)
{
    this->drawFastVLine(xOffset, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset + Y_AXIS_LENGTH, X_AXIS_LENGTH, AXIS_COLOR);
}

void LGFX::refreshTemperatureMeter(uint8_t xOffset, uint8_t yOffset, uint8_t temperature)
{
    this->temperatureSprite->scroll(-1, 0);
    int32_t color = this->getGradientColor(temperature);
    this->temperatureSprite->drawFastVLine(X_AXIS_LENGTH - 1, Y_AXIS_LENGTH - 2 - (temperature / 2), (temperature / 2), color);
    this->setCursor(xOffset + SCREEN_WIDTH - 80, yOffset);
    this->setTextSize(3);
    this->setTextColor(color, TFT_BLACK);
    this->setTextWrap(true);
    this->printf("%02dc", temperature);
    this->temperatureSprite->pushSprite(xOffset + 3, yOffset);
}