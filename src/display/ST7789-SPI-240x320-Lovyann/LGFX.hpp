#include <LovyanGFX.hpp>
#include <stdint.h>

class LGFX : public lgfx::LGFX_Device
{
private:
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::LGFX_Sprite *temperatureSprite = nullptr;

    uint32_t getTemperatureGradientColor(int8_t value);

public:
    LGFX(uint8_t SDA, uint8_t SCL, uint8_t CS, uint8_t DC, uint8_t RST);
    ~LGFX();
    void initCPUMeter(uint8_t xOffset, uint8_t yOffset);
    void initMemoryMeter(uint8_t xOffset, uint8_t yOffset);
    void initNetworkMeter(uint8_t xOffset, uint8_t yOffset);
    void initTemperatureMeter(uint8_t xOffset, uint8_t yOffset);

    void refreshTemperatureMeter(uint8_t xOffset, uint8_t yOffset, uint8_t temperature);
};