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
#define DEBUG_SPRITE_HEIGHT 13
#define DEBUG_SPRITE_BACKGROUND TFT_BLACK

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

void LGFX::addMeter(LGFXMeter *meter)
{
}

void LGFX::setSource(ISource *src)
{
    this->source = src;
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
    char unit[8] = "";

    if (millis_diff >= millis_in_year)
    {
        time = (double)millis_diff / millis_in_year;
        strcpy(unit, time > 1 ? "years" : "year");
    }
    else if (millis_diff >= millis_in_month)
    {
        time = (double)millis_diff / millis_in_month;
        strcpy(unit, time > 1 ? "months" : " month");
    }
    else if (millis_diff >= millis_in_day)
    {
        time = (double)millis_diff / millis_in_day;
        strcpy(unit, time > 1 ? "days" : "day");
    }
    else if (millis_diff >= millis_in_hour)
    {
        time = (double)millis_diff / millis_in_hour;
        strcpy(unit, time > 1 ? "hours" : "hour");
    }
    else if (millis_diff >= millis_in_minute)
    {
        time = (double)millis_diff / millis_in_minute;
        strcpy(unit, time > 1 ? "mins" : "min");
    }
    else
    {
        time = (double)millis_diff / millis_in_second;
        strcpy(unit, time > 1 ? "secs" : "sec");
    }
    snprintf(buffer, buffer_size, "%.1f %s", time, unit);
}

void LGFX::refreshDebug(uint16_t xOffset, uint16_t yOffset, int32_t wifiSignalStrength)
{
    this->fpsDebug->loop();

    this->debugSprite->fillSprite(DEBUG_SPRITE_BACKGROUND);
    this->debugSprite->setTextSize(1);
    this->debugSprite->setTextColor(TFT_WHITE, TFT_BLACK);
    this->debugSprite->setCursor(0, 0);
    char timeString[50];
    convertMillisToString(millis() - this->startMillis, timeString, sizeof(timeString));
    this->debugSprite->printf("Runtime: %s - FPS: %03u - Wifi: %03ddBm", timeString, this->fpsDebug->getFPS(), wifiSignalStrength);
    /*
        RSSI > - 30 dBm	 Amazing
        RSSI < – 55 dBm	 Very good signal
        RSSI < – 67 dBm	 Fairly Good
        RSSI < – 70 dBm	 Okay
        RSSI < – 80 dBm	 Not good
        RSSI < – 90 dBm	 Extremely weak signal (unusable)
    */
    this->debugSprite->pushSprite(xOffset, yOffset);
}

void LGFX::refreshInfo(uint64_t runtime, bool WIFIConnected, IPAddress ip, uint8_t mac[6], int WifidBSignal)
{
    this->setTextSize(1);
    this->setTextColor(TFT_WHITE, TFT_BLACK);
    this->setCursor(0, 0);
    this->println("STATUS");
    this->printf("RUNTINE (ms): %llu\n", runtime);
    this->printf("WIFI STATUS: %s\n", WIFIConnected ? "CONNECTED      " : "NOT CONNECTED");

    if (WIFIConnected)
    {
        this->printf("WIFI MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        this->printf("WIFI IP: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
        this->printf("WIFI SIGNAL: %03ddBm\n", WifidBSignal);
    }
    else
    {
        this->println("WIFI MAC:                                  ");
        this->println("WIFI IP:                                   ");
        this->println("WIFI SIGNAL:                               ");
    }

    int32_t colors[6];
    if (WIFIConnected)
    {
        if (WifidBSignal < -90)
        {
            colors[0] = TFT_GREEN;
            colors[1] = colors[2] = colors[3] = colors[4] = colors[5] = TFT_DARKGREY;
        }
        else if (WifidBSignal < -80)
        {
            colors[0] = colors[1] = TFT_GREEN;
            colors[2] = colors[3] = colors[4] = colors[5] = TFT_DARKGREY;
        }
        else if (WifidBSignal < -70)
        {
            colors[0] = colors[1] = colors[2] = TFT_GREEN;
            colors[3] = colors[4] = colors[5] = TFT_DARKGREY;
        }
        else if (WifidBSignal < -67)
        {
            colors[0] = colors[1] = colors[2] = colors[3] = TFT_GREEN;
            colors[4] = colors[5] = TFT_DARKGREY;
        }
        else if (WifidBSignal < -55)
        {
            colors[0] = colors[1] = colors[2] = colors[3] = colors[4] = TFT_GREEN;
            colors[5] = TFT_DARKGREY;
        }
        else if (WifidBSignal > -30)
        {
            colors[0] = colors[1] = colors[2] = colors[3] = colors[4] = colors[5] = TFT_GREEN;
        }
    }
    else
    {
        colors[0] = colors[1] = colors[2] = colors[3] = colors[4] = colors[5] = TFT_GREEN;
    }
    uint16_t x = 100;
    uint16_t height = 20;
    for (uint8_t i = 0; i < 6; i++)
    {
        this->fillRect(x, 100, 10, height, colors[i]);
        x += 20;
        height += 10;
    }
}