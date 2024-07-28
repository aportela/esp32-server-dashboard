#include "LGFXScreen.hpp"

// #define DEBUG_SCREEN_TITLE_BOUNDS

#ifdef DEBUG_SCREEN_TITLE_BOUNDS

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 320
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 240
#endif

#endif

LGFXScreen::LGFXScreen(LovyanGFX *display) : parentDisplay(display)
{
    if (display != nullptr)
    {
        this->parentDisplay->fillScreen(TFT_BLACK);
        this->parentDisplay->setTextWrap(false);
#ifdef DEBUG_SCREEN_TITLE_BOUNDS
        this->parentDisplay->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, TFT_WHITE);
#endif
    }
}

LGFXScreen::~LGFXScreen()
{
    if (this->parentDisplay != nullptr)
    {
        this->parentDisplay = nullptr;
    }
}
