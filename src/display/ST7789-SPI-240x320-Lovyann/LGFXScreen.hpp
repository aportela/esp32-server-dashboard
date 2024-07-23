#ifndef ESP32_SERVER_DASHBOARD_LGFX_SCREEN_H
#define ESP32_SERVER_DASHBOARD_LGFX_SCREEN_H

#include <LovyanGFX.hpp>
#include "../IScreen.hpp"
#include "../FPSDebug.hpp"

class LGFXScreen : public IScreen
{
protected:
    LovyanGFX *parentDisplay = nullptr;
    FPSDebug *fpsDebug;

public:
    LGFXScreen(LovyanGFX *display, FPSDebug *fpsDebug);
    ~LGFXScreen();
    virtual void refresh(bool firstRun) = 0;
};

#endif