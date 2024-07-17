#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <stdint.h>

#define TFT_CS 5
#define TFT_RST 9
#define TFT_DC 8
#define TFT_SDA 6
#define TFT_SCL 4

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
LGFX *screen = nullptr;
lgfx::LGFX_Sprite *graphSprite = nullptr;

uint8_t values[200];

uint32_t getGradientColor(int value, LGFX* screen)
{
    // Asegurarse de que el valor esté dentro del rango 0-100
    value = constrain(value, 0, 100);

    // Azul muy oscuro a verde (0 a 33)
    if (value <= 33)
    {
        int blue = 255;
        int green = map(value, 0, 33, 0, 255);
        return screen->color565(0, green, blue);
    }
    // Verde a amarillo (34 a 66)
    else if (value <= 66)
    {
        int green = 255;
        int red = map(value, 34, 66, 0, 255);
        return screen->color565(red, green, 0);
    }
    // Amarillo a rojo (67 a 100)
    else
    {
        int red = 255;
        int green = map(value, 67, 100, 255, 0);
        return screen->color565(red, green, 0);
    }
}

void setup()
{
    screen = new LGFX();
    screen->init();
    screen->fillScreen(TFT_BLACK);
    graphSprite = new lgfx::LGFX_Sprite(screen);
    graphSprite->createSprite(200, 100);
    graphSprite->fillSprite(TFT_BLACK);
    screen->drawFastVLine(10, 100, 100, TFT_WHITE);
    screen->drawFastHLine(10, 200, 200, TFT_WHITE);
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
    for (int i = 0; i < 200; i++)
    {
        if (i < 199) // move data array to left
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
        }
    }
    // move graph sprite to left
    graphSprite->scroll(-1, 0);
    int32_t color = 0;
    for (int i = 0; i < 200; i++)
    {
        if (values[i] > 0)
        {
            color = getGradientColor(values[i], screen);
            graphSprite->drawFastVLine(i, 98 - values[i], values[i], color);
        }
    }

    screen->setCursor(200, 10);
    screen->setTextSize(5);
    screen->setTextColor(color, TFT_BLACK);
    screen->setTextWrap(true);
    screen->printf("%02dc", values[199]);
    graphSprite->pushSprite(12, 100);
}
