#ifndef ESP32_SERVER_DASHBOARD_LGFX_METER_H
#define ESP32_SERVER_DASHBOARD_LGFX_METER_H

#include <stdint.h>
#include <LovyanGFX.hpp>

class LGFXMeter
{
public:
    LGFXMeter(LovyanGFX *parent);
    ~LGFXMeter();
};

#endif
