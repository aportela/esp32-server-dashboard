#include "LGFX.hpp"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define X_AXIS_LENGTH 204
#define Y_AXIS_LENGTH 54

#define AXIS_COLOR TFT_WHITE

#define GRAPH_SPRITE_WIDTH 200
#define GRAPH_SPRITE_HEIGHT 50
#define GRAPH_SPRITE_BACKGROUND TFT_BLACK

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

    this->cpuLoadSprite = new lgfx::LGFX_Sprite(this);
    this->cpuLoadSprite->createSprite(GRAPH_SPRITE_WIDTH, GRAPH_SPRITE_HEIGHT);
    this->cpuLoadSprite->fillSprite(GRAPH_SPRITE_BACKGROUND);

    this->memorySprite = new lgfx::LGFX_Sprite(this);
    this->memorySprite->createSprite(GRAPH_SPRITE_WIDTH, GRAPH_SPRITE_HEIGHT);
    this->memorySprite->fillSprite(GRAPH_SPRITE_BACKGROUND);

    this->temperatureSprite = new lgfx::LGFX_Sprite(this);
    this->temperatureSprite->createSprite(GRAPH_SPRITE_WIDTH, GRAPH_SPRITE_HEIGHT);
    this->temperatureSprite->fillSprite(GRAPH_SPRITE_BACKGROUND);
    this->startMillis = millis();
}

LGFX::~LGFX()
{
    delete this->cpuLoadSprite;
    this->cpuLoadSprite = nullptr;
    delete this->memorySprite;
    this->memorySprite = nullptr;
    delete this->temperatureSprite;
    this->temperatureSprite = nullptr;
}

uint32_t LGFX::getTemperatureGradientColor(int8_t value)
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

void LGFX::initCPULoadMeter(uint8_t xOffset, uint8_t yOffset)
{
    // TODO: check axis & sprite bounds
    this->drawFastVLine(xOffset, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset + Y_AXIS_LENGTH, X_AXIS_LENGTH, AXIS_COLOR);

    this->drawFastVLine(xOffset + X_AXIS_LENGTH, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset, X_AXIS_LENGTH, AXIS_COLOR);

    this->setCursor(xOffset + SCREEN_WIDTH - 110, yOffset);
    this->setTextSize(2);
    this->setTextColor(TFT_WHITE, TFT_BLACK);
    this->setTextWrap(true);
    this->print("CPU LOAD");
    this->setTextSize(2);
    this->setCursor(xOffset + SCREEN_WIDTH - 32, yOffset + 28);
    this->print("%");
}

void LGFX::refreshCPULoadMeter(uint8_t xOffset, uint8_t yOffset, uint8_t load)
{
    this->cpuLoadSprite->scroll(-1, 0);
    int32_t color = this->getTemperatureGradientColor(load);
    // TODO: check axis & sprite bounds
    this->cpuLoadSprite->drawFastVLine(GRAPH_SPRITE_WIDTH - 1, GRAPH_SPRITE_HEIGHT - (load / 2) + 1, (load / 2), color);
    this->setCursor(xOffset + SCREEN_WIDTH - 92, yOffset + 30);
    this->setTextSize(3);
    this->setTextColor(color, TFT_BLACK);
    this->setTextWrap(true);
    this->printf("%03d", load);
    this->cpuLoadSprite->pushSprite(xOffset + 2, yOffset + 2);
}

void LGFX::initMemoryMeter(uint8_t xOffset, uint8_t yOffset)
{
    // TODO: check axis & sprite bounds
    this->drawFastVLine(xOffset, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset + Y_AXIS_LENGTH, X_AXIS_LENGTH, AXIS_COLOR);

    this->drawFastVLine(xOffset + X_AXIS_LENGTH, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset, X_AXIS_LENGTH, AXIS_COLOR);

    this->setCursor(xOffset + SCREEN_WIDTH - 100, yOffset);
    this->setTextSize(2);
    this->setTextColor(TFT_WHITE, TFT_BLACK);
    this->setTextWrap(true);
    this->print("MEMORY");
    this->setTextSize(2);
    this->setCursor(xOffset + SCREEN_WIDTH - 44, yOffset + 21);
    this->print("Gb");
    this->setCursor(xOffset + SCREEN_WIDTH - 92, yOffset + 40);
    this->printf("%03d Gb", 32);
}

void LGFX::refreshMemoryMeter(uint8_t xOffset, uint8_t yOffset, uint64_t totalMemory, uint64_t usedMemory)
{
    uint8_t interpolatedMemory = map(usedMemory, 0, totalMemory, 0, 100);
    this->memorySprite->scroll(-1, 0);
    int32_t color = this->getTemperatureGradientColor(interpolatedMemory);
    // TODO: check axis & sprite bounds
    this->memorySprite->drawFastVLine(GRAPH_SPRITE_WIDTH - 1, GRAPH_SPRITE_HEIGHT - (interpolatedMemory / 2) + 1, (interpolatedMemory / 2), color);
    this->setCursor(xOffset + SCREEN_WIDTH - 92, yOffset + 21);
    this->setTextSize(2);
    this->setTextColor(color, TFT_BLACK);
    this->setTextWrap(true);
    this->printf("%03d", usedMemory);
    this->memorySprite->pushSprite(xOffset + 2, yOffset + 2);
}

void LGFX::initNetworkMeter(uint8_t xOffset, uint8_t yOffset)
{
}

void LGFX::initTemperatureMeter(uint8_t xOffset, uint8_t yOffset)
{
    // TODO: check axis & sprite bounds
    this->drawFastVLine(xOffset, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset + Y_AXIS_LENGTH, X_AXIS_LENGTH, AXIS_COLOR);

    this->drawFastVLine(xOffset + X_AXIS_LENGTH, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset, X_AXIS_LENGTH, AXIS_COLOR);

    this->setCursor(xOffset + SCREEN_WIDTH - 110, yOffset);
    this->setTextSize(2);
    this->setTextColor(TFT_WHITE, TFT_BLACK);
    this->setTextWrap(true);
    this->print("CPU TEMP");
    this->setTextSize(2);
    this->setCursor(xOffset + SCREEN_WIDTH - 32, yOffset + 28);
    this->print("c");
}

void LGFX::refreshTemperatureMeter(uint8_t xOffset, uint8_t yOffset, uint8_t temperature)
{
    this->temperatureSprite->scroll(-1, 0);
    int32_t color = this->getTemperatureGradientColor(temperature);
    // TODO: check axis & sprite bounds
    this->temperatureSprite->drawFastVLine(GRAPH_SPRITE_WIDTH - 1, GRAPH_SPRITE_HEIGHT - (temperature / 2) + 1, (temperature / 2), color);
    this->setCursor(xOffset + SCREEN_WIDTH - 92, yOffset + 30);
    this->setTextSize(3);
    this->setTextColor(color, TFT_BLACK);
    this->setTextWrap(true);
    this->printf("%03d", temperature);
    this->temperatureSprite->pushSprite(xOffset + 2, yOffset + 2);
}

void LGFX::refreshDebug(uint8_t xOffset, uint8_t yOffset)
{
    this->setCursor(xOffset, yOffset);
    this->setTextSize(1);
    this->setTextColor(TFT_WHITE, TFT_BLACK);
    this->setTextWrap(true);
    this->printf("Running seconds: %08d", (millis() - this->startMillis) / 1000);
}