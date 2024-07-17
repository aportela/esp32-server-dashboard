#include <Arduino.h>
#include <stdint.h>

#define TFT_CS 5
#define TFT_RST 9
#define TFT_DC 8
#define TFT_SDA 6
#define TFT_SCL 4

// #define ADAFRUIT_SPI_SOFTWARE

#define LOVYAN_SPI

#ifdef ADAFRUIT_SPI_SOFTWARE

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

Adafruit_ST7789 *screen = nullptr;

#endif

#ifdef LOVYAN_SPI

#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;

public:
    LGFX(void)
    {
        auto cfg = _bus_instance.config();
        cfg.spi_host = SPI2_HOST;
        cfg.spi_mode = 0;
        cfg.freq_write = 40000000;
        cfg.freq_read = 16000000;
        cfg.spi_3wire = false;
        cfg.use_lock = true;
        cfg.dma_channel = 1;
        cfg.pin_sclk = TFT_SCL;
        cfg.pin_mosi = TFT_SDA;
        cfg.pin_miso = -1;
        cfg.pin_dc = TFT_DC;
        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);

        auto cfg_panel = _panel_instance.config();
        cfg_panel.pin_cs = TFT_CS;
        cfg_panel.pin_rst = TFT_RST;
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
    }
};
LGFX *screen;

#endif

uint8_t values[200];

void setup()
{
#ifdef ADAFRUIT_SPI_SOFTWARE
    screen = new Adafruit_ST7789(TFT_CS, TFT_DC, TFT_SDA, TFT_SCL, TFT_RST);
    screen->init(240, 320);
    screen->setRotation(1);
    screen->fillScreen(ST77XX_BLACK);
    screen->drawFastVLine(10, 100, 100, ST77XX_WHITE);
    screen->drawFastHLine(10, 200, 200, ST77XX_WHITE);
#endif
#ifdef LOVYAN_SPI
    screen = new LGFX();
    screen->init();
    screen->fillScreen(TFT_BLACK);
    screen->drawFastVLine(10, 100, 100, TFT_WHITE);
    screen->drawFastHLine(10, 200, 200, TFT_WHITE);
#endif
    for (int i = 0; i < 200; i++)
    {
        values[i] = 0;
    }
    // init random seed
    randomSeed(analogRead(0) ^ (micros() * esp_random()));
    values[199] = random(20, 99);
}

void loop()
{
    // clear old & reload new
    for (int i = 0; i < 200; i++)
    {
        if (values[i] > 0)
        {
#ifdef ADAFRUIT_SPI_SOFTWARE
            // screen->drawPixel(i + 12, 199 - values[i], ST77XX_BLACK);
            screen->drawFastVLine(i + 12, 198 - values[i], values[i], ST77XX_BLACK);
#endif
#ifdef LOVYAN_SPI
            // screen->drawPixel(i + 12, 199 - values[i], TFT_BLACK);
            screen->drawFastVLine(i + 12, 198 - values[i], values[i], TFT_BLACK);
#endif
        }
        if (i < 199)
        {
            values[i] = values[i + 1];
        }
        else
        {
            if (random(0, 20) % 2 == 0)
            {
                if (values[199] < 99)
                {
                    values[199]++;
                }
            }
            else if (values[199] > 1)
            {
                values[199]--;
            }
            else
            {
            }
        }
    }
    int32_t color = 0;
    for (int i = 0; i < 200; i++)
    {
        if (values[i] > 0)
        {
#ifdef ADAFRUIT_SPI_SOFTWARE
            // screen->drawPixel(i + 12, 199 - values[i], ST77XX_RED);
            screen->drawFastVLine(i + 12, 198 - values[i], values[i], ST77XX_RED);
#endif
#ifdef LOVYAN_SPI
            // screen->drawPixel(i + 12, 199 - values[i], TFT_RED);
            if (values[i] < 20)
            {
                color = TFT_BLUE;
            }
            else if (values[i] < 40)
            {
                color = TFT_GREEN;
            }
            else if (values[i] < 50)
            {
                color = TFT_YELLOW;
            }
            else if (values[i] < 70)
            {
                color = TFT_ORANGE;
            }
            else
            {
                color = TFT_RED;
            }
            screen->drawFastVLine(i + 12, 198 - values[i], values[i], color);
#endif
        }
    }
#ifdef ADAFRUIT_SPI_SOFTWARE
    screen->setCursor(200, 10);
    screen->setTextSize(5);
    screen->setTextColor(ST77XX_BLUE, ST77XX_BLACK);
    screen->setTextWrap(true);
    screen->printf("%02dc", values[199]);
#endif
#ifdef LOVYAN_SPI
    screen->setCursor(200, 10);
    screen->setTextSize(5);
    screen->setTextColor(color, TFT_BLACK);
    screen->setTextWrap(true);
    screen->printf("%02dc", values[199]);
#endif
}
