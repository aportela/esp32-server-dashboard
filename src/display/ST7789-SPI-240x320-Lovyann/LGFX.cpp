#include "LGFX.hpp"
#include "../../sources/dummy/DummySource.hpp"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define GRAPH_SPRITE_WIDTH 200
#define GRAPH_SPRITE_HEIGHT 30
#define GRAPH_SPRITE_BACKGROUND TFT_BLACK

#define X_AXIS_LENGTH GRAPH_SPRITE_WIDTH + 4
#define Y_AXIS_LENGTH GRAPH_SPRITE_HEIGHT + 4

#define AXIS_COLOR TFT_WHITE

#define GRAPH_LABEL_FONT_SIZE 1

#define DEBUG_SPRITE_WIDTH SCREEN_WIDTH - 20
#define DEBUG_SPRITE_HEIGHT 10
#define DEBUG_SPRITE_BACKGROUND TFT_BLACK

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

    this->cpuTemperatureSprite = new lgfx::LGFX_Sprite(this);
    this->cpuTemperatureSprite->createSprite(GRAPH_SPRITE_WIDTH, GRAPH_SPRITE_HEIGHT);
    this->cpuTemperatureSprite->fillSprite(GRAPH_SPRITE_BACKGROUND);

    this->networkDownloadSprite = new lgfx::LGFX_Sprite(this);
    this->networkDownloadSprite->createSprite(GRAPH_SPRITE_WIDTH, GRAPH_SPRITE_HEIGHT);
    this->networkDownloadSprite->fillSprite(GRAPH_SPRITE_BACKGROUND);

    this->networkUploadSprite = new lgfx::LGFX_Sprite(this);
    this->networkUploadSprite->createSprite(GRAPH_SPRITE_WIDTH, GRAPH_SPRITE_HEIGHT);
    this->networkUploadSprite->fillSprite(GRAPH_SPRITE_BACKGROUND);

    this->debugSprite = new lgfx::LGFX_Sprite(this);
    this->debugSprite->createSprite(DEBUG_SPRITE_WIDTH, DEBUG_SPRITE_HEIGHT);
    this->debugSprite->setFont(&fonts::Font2);

    this->fpsDebug = new FPSDebug();

    this->setFont(&fonts::Font2);
    this->startMillis = millis();
}

LGFX::~LGFX()
{
    delete this->cpuLoadSprite;
    this->cpuLoadSprite = nullptr;
    delete this->memorySprite;
    this->memorySprite = nullptr;
    delete this->cpuTemperatureSprite;
    this->cpuTemperatureSprite = nullptr;
    delete this->networkDownloadSprite;
    this->networkDownloadSprite = nullptr;
    delete this->networkUploadSprite;
    this->networkUploadSprite = nullptr;
    delete this->fpsDebug;
    this->fpsDebug = nullptr;
}

uint32_t LGFX::getTemperatureGradientFrom0To100(int8_t value)
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

    this->drawFastVLine(xOffset + X_AXIS_LENGTH - 1, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset, X_AXIS_LENGTH, AXIS_COLOR);

    this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset);
    this->setTextSize(GRAPH_LABEL_FONT_SIZE);
    this->setTextColor(TFT_WHITE, TFT_BLACK);
    this->print("CPU LOAD");
}

void LGFX::refreshCPULoadMeter(uint8_t xOffset, uint8_t yOffset, uint8_t load)
{
    // TODO: check axis & sprite bounds
    uint8_t mapped100 = map(load, MIN_CPU_LOAD, MAX_CPU_LOAD, 0, 100);
    int32_t gradientColor = this->getTemperatureGradientFrom0To100(mapped100);
    uint8_t mappedGraphValue = map(load, MIN_CPU_LOAD, MAX_CPU_LOAD, 0, GRAPH_SPRITE_HEIGHT);
    // create graph animation moving sprite to left 1 pixel
    this->cpuLoadSprite->scroll(-1, 0);
    // draw new value (on right)
    this->cpuLoadSprite->drawFastVLine(GRAPH_SPRITE_WIDTH - 1, GRAPH_SPRITE_HEIGHT - mappedGraphValue + 1, mappedGraphValue, gradientColor);
    if (load != this->oldCPULoad)
    {
        this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset + 20);
        this->setTextSize(GRAPH_LABEL_FONT_SIZE);
        this->setTextColor(gradientColor, TFT_BLACK);
        this->printf("%03d%%", load);
        this->cpuLoadSprite->pushSprite(xOffset + 2, yOffset + 2);
        this->oldCPULoad = load;
    }
}

void LGFX::initMemoryMeter(uint8_t xOffset, uint8_t yOffset)
{
    // TODO: check axis & sprite bounds
    this->drawFastVLine(xOffset, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset + Y_AXIS_LENGTH, X_AXIS_LENGTH, AXIS_COLOR);

    this->drawFastVLine(xOffset + X_AXIS_LENGTH - 1, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset, X_AXIS_LENGTH, AXIS_COLOR);

    this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset);
    this->setTextSize(GRAPH_LABEL_FONT_SIZE);
    this->setTextColor(TFT_WHITE, TFT_BLACK);
    this->print("USED Memory");
    this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset + 20);
    this->print("000GB / 032GB");
}

void LGFX::refreshMemoryMeter(uint8_t xOffset, uint8_t yOffset, uint64_t usedMemory)
{
    // TODO: check axis & sprite MAX
    uint8_t mapped100 = map(usedMemory, MIN_MEMORY, MAX_MEMORY, 0, 100);
    int32_t gradientColor = this->getTemperatureGradientFrom0To100(mapped100);
    uint8_t mappedGraphValue = map(usedMemory, MIN_MEMORY, MAX_MEMORY, 0, GRAPH_SPRITE_HEIGHT);
    // create graph animation moving sprite to left 1 pixel
    this->memorySprite->scroll(-1, 0);
    // draw new value (on right)
    this->memorySprite->drawFastVLine(GRAPH_SPRITE_WIDTH - 1, GRAPH_SPRITE_HEIGHT - mappedGraphValue + 1, mappedGraphValue, gradientColor);
    this->memorySprite->pushSprite(xOffset + 2, yOffset + 2);
    if (usedMemory != this->oldUsedMemory)
    {
        this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset + 20);
        this->setTextSize(GRAPH_LABEL_FONT_SIZE);
        this->setTextColor(gradientColor, TFT_BLACK);
        this->printf("%03d", usedMemory);
        this->print("GB");
        this->oldUsedMemory = usedMemory;
    }
}

void LGFX::initCPUTemperatureMeter(uint8_t xOffset, uint8_t yOffset)
{
    // TODO: check axis & sprite bounds
    this->drawFastVLine(xOffset, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset + Y_AXIS_LENGTH, X_AXIS_LENGTH, AXIS_COLOR);

    this->drawFastVLine(xOffset + X_AXIS_LENGTH - 1, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset, X_AXIS_LENGTH, AXIS_COLOR);

    this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset);
    this->setTextSize(GRAPH_LABEL_FONT_SIZE);
    this->setTextColor(TFT_WHITE, TFT_BLACK);
    this->print("CPU Temperature");
}

void LGFX::refreshCPUTemperatureMeter(uint8_t xOffset, uint8_t yOffset, uint8_t temperature)
{
    // TODO: check axis & sprite bounds
    uint8_t mapped100 = map(temperature, MIN_CPU_TEMPERATURE, MAX_CPU_TEMPERATURE, 0, 100);
    int32_t gradientColor = this->getTemperatureGradientFrom0To100(mapped100);
    uint8_t mappedGraphValue = map(temperature, MIN_CPU_TEMPERATURE, MAX_CPU_TEMPERATURE, 0, GRAPH_SPRITE_HEIGHT);
    // create graph animation moving sprite to left 1 pixel
    this->cpuTemperatureSprite->scroll(-1, 0);
    // draw new value (on right)
    this->cpuTemperatureSprite->drawFastVLine(GRAPH_SPRITE_WIDTH - 1, GRAPH_SPRITE_HEIGHT - mappedGraphValue + 1, mappedGraphValue, gradientColor);
    this->cpuTemperatureSprite->pushSprite(xOffset + 2, yOffset + 2);
    if (temperature != this->oldCPUTemperature)
    {
        this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset + 20);
        this->setTextSize(GRAPH_LABEL_FONT_SIZE);
        this->setTextColor(gradientColor, TFT_BLACK);
        this->printf("%03dC", temperature);
        this->oldCPUTemperature = temperature;
    }
}

void LGFX::initNetworkDownloadBandwithMeter(uint8_t xOffset, uint8_t yOffset)
{
    // TODO: check axis & sprite bounds
    this->drawFastVLine(xOffset, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset + Y_AXIS_LENGTH, X_AXIS_LENGTH, AXIS_COLOR);

    this->drawFastVLine(xOffset + X_AXIS_LENGTH - 1, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset, X_AXIS_LENGTH, AXIS_COLOR);

    this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset);
    this->setTextSize(GRAPH_LABEL_FONT_SIZE);
    this->setTextColor(TFT_WHITE, TFT_BLACK);
    this->print("WAN Download");
    // TODO: fixed max
    this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset + 20);
    this->print("000MB / 512MB");
}

void LGFX::refreshNetworkDownloadBandwithMeter(uint8_t xOffset, uint8_t yOffset, uint64_t bandwith)
{
    uint64_t bandwithHumanIntValue = bandwith;
    uint16_t bandwithHumanIntDecimalValue = bandwith > 999 ? bandwith % 1000 : 0;
    char *units[] = {"B  ", "KB ", "MB ", "GB ", "TB ", "PB "};
    uint8_t currentUnitIndex = 0;
    while (bandwithHumanIntValue > 999)
    {
        bandwithHumanIntValue = bandwithHumanIntValue / 1000;
        currentUnitIndex++;
    }
    // TODO: check axis & sprite MAX
    // TODO: fixed max
    uint8_t mapped100 = currentUnitIndex > 0 ? map(bandwithHumanIntValue, 0, 512, 0, 100) : 0;
    int32_t gradientColor = this->getTemperatureGradientFrom0To100(mapped100);
    uint8_t mappedGraphValue = currentUnitIndex > 0 ? map(bandwithHumanIntValue, 0, 512, 0, GRAPH_SPRITE_HEIGHT) : 0;
    // create graph animation moving sprite to left 1 pixel
    this->networkDownloadSprite->scroll(-1, 0);
    // draw new value (on right)
    this->networkDownloadSprite->drawFastVLine(GRAPH_SPRITE_WIDTH - 1, GRAPH_SPRITE_HEIGHT - mappedGraphValue + 1, mappedGraphValue, gradientColor);
    this->networkDownloadSprite->pushSprite(xOffset + 2, yOffset + 2);
    if (bandwith != this->oldNetworkDownloadBandwith)
    {
        this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset + 20);
        this->setTextSize(GRAPH_LABEL_FONT_SIZE);
        this->setTextColor(gradientColor, TFT_BLACK);
        this->printf("%03d", bandwithHumanIntValue);
        this->print(units[currentUnitIndex]);
        this->oldNetworkDownloadBandwith = bandwith;
    }
}

void LGFX::initNetworkUploadBandwithMeter(uint8_t xOffset, uint8_t yOffset)
{
    // TODO: check axis & sprite bounds
    this->drawFastVLine(xOffset, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset + Y_AXIS_LENGTH, X_AXIS_LENGTH, AXIS_COLOR);

    this->drawFastVLine(xOffset + X_AXIS_LENGTH - 1, yOffset, Y_AXIS_LENGTH, AXIS_COLOR);
    this->drawFastHLine(xOffset, yOffset, X_AXIS_LENGTH, AXIS_COLOR);

    this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset);
    this->setTextSize(GRAPH_LABEL_FONT_SIZE);
    this->setTextColor(TFT_WHITE, TFT_BLACK);
    this->print("WAN Upload");
    // TODO: fixed max
    this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset + 20);
    this->print("000MB / 512MB");
}

void LGFX::refreshNetworkUploadBandwithMeter(uint8_t xOffset, uint8_t yOffset, uint64_t bandwith)
{
    uint64_t bandwithHumanIntValue = bandwith;
    uint16_t bandwithHumanIntDecimalValue = bandwith > 999 ? bandwith % 1000 : 0;
    char *units[] = {"B  ", "KB ", "MB ", "GB ", "TB ", "PB "};
    uint8_t currentUnitIndex = 0;
    while (bandwithHumanIntValue > 999)
    {
        bandwithHumanIntValue = bandwithHumanIntValue / 1000;
        currentUnitIndex++;
    }
    // TODO: check axis & sprite MAX
    // TODO: fixed max
    uint8_t mapped100 = currentUnitIndex > 0 ? map(bandwithHumanIntValue, 0, 512, 0, 100) : 0;
    int32_t gradientColor = this->getTemperatureGradientFrom0To100(mapped100);
    uint8_t mappedGraphValue = currentUnitIndex > 0 ? map(bandwithHumanIntValue, 0, 512, 0, GRAPH_SPRITE_HEIGHT) : 0;
    // create graph animation moving sprite to left 1 pixel
    this->networkUploadSprite->scroll(-1, 0);
    // draw new value (on right)
    this->networkUploadSprite->drawFastVLine(GRAPH_SPRITE_WIDTH - 1, GRAPH_SPRITE_HEIGHT - mappedGraphValue + 1, mappedGraphValue, gradientColor);
    this->networkUploadSprite->pushSprite(xOffset + 2, yOffset + 2);
    if (bandwith != this->oldNetworkUploadBandwith)
    {
        this->setCursor(xOffset + SCREEN_WIDTH - 105, yOffset + 20);
        this->setTextSize(GRAPH_LABEL_FONT_SIZE);
        this->setTextColor(gradientColor, TFT_BLACK);
        this->printf("%03d", bandwithHumanIntValue);
        this->print(units[currentUnitIndex]);
        this->oldNetworkUploadBandwith = bandwith;
    }
}

void convertMillisToString(unsigned long long millis_diff, char *buffer, size_t buffer_size)
{
    const unsigned long long millis_in_second = 1000;
    const unsigned long long millis_in_minute = millis_in_second * 60;
    const unsigned long long millis_in_hour = millis_in_minute * 60;
    const unsigned long long millis_in_day = millis_in_hour * 24;
    const unsigned long long millis_in_month = millis_in_day * 30;
    const unsigned long long millis_in_year = millis_in_day * 365;

    double time = 0.0;
    char unit[10] = "";

    if (millis_diff >= millis_in_year)
    {
        time = (double)millis_diff / millis_in_year;
        strcpy(unit, " year/s");
    }
    else if (millis_diff >= millis_in_month)
    {
        time = (double)millis_diff / millis_in_month;
        strcpy(unit, " month/s");
    }
    else if (millis_diff >= millis_in_day)
    {
        time = (double)millis_diff / millis_in_day;
        strcpy(unit, " day/s");
    }
    else if (millis_diff >= millis_in_hour)
    {
        time = (double)millis_diff / millis_in_hour;
        strcpy(unit, " hour/s");
    }
    else if (millis_diff >= millis_in_minute)
    {
        time = (double)millis_diff / millis_in_minute;
        strcpy(unit, " minute/s");
    }
    else
    {
        time = (double)millis_diff / millis_in_second;
        strcpy(unit, " second/s");
    }
    snprintf(buffer, buffer_size, "%.1f%s", time, unit);
}

void LGFX::refreshDebug(uint8_t xOffset, uint8_t yOffset)
{
    this->fpsDebug->loop();

    this->debugSprite->fillSprite(DEBUG_SPRITE_BACKGROUND);
    this->debugSprite->setTextSize(1);
    this->debugSprite->setTextColor(TFT_WHITE, TFT_BLACK);
    this->debugSprite->setCursor(0, 0);
    char timeString[50];
    convertMillisToString(millis() - this->startMillis, timeString, sizeof(timeString));
    this->debugSprite->printf("Dashboard uptime: %s - FPS: %03u", timeString, this->fpsDebug->getFPS());
    this->debugSprite->pushSprite(xOffset, yOffset);
}
