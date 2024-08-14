#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_H

#include <LovyanGFX.hpp>
#include "../IScreen.hpp"

class LGFXScreen : public IScreen
{
protected:
    LovyanGFX *parentDisplay = nullptr;

public:
    LGFXScreen(LovyanGFX *display);
    ~LGFXScreen();
    virtual bool refresh(bool force = false) = 0;
};

#endif
