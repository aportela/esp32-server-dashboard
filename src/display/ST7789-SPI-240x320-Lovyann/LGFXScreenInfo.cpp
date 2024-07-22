#include "LGFXScreenInfo.hpp"
#include "../../utils/FormatUtils.hpp"

#define MAIN_SPRITE_WIDTH 312
#define MAIN_SPRITE_HEIGHT 216

LGFXScreenInfo::LGFXScreenInfo(LovyanGFX *display, FPSDebug *fpsDebug, SystemStatus *sysStats) : LGFXScreen(display, fpsDebug, sysStats)
{

    this->parentDisplay->drawRect(0, 0, 320, 240, TFT_WHITE);
    this->parentDisplay->setFont(&fonts::FreeMono9pt7b);
    this->parentDisplay->setTextSize(1);
    this->parentDisplay->setTextColor(TFT_WHITE);
    this->parentDisplay->setCursor(40, 4);
    this->parentDisplay->println("ESP32-SERVER-DASBBOARD");

    this->refreshWIFILogo = true;
    this->refreshWIFISignalStrength = true;
    this->refreshWIFISignalLevelGraph = true;
    this->refreshWIFIData = true;
}

LGFXScreenInfo::~LGFXScreenInfo()
{
}

void LGFXScreenInfo::refresh(void)
{
    this->fpsDebug->loop();
    this->sysStats->refresh();

    if (this->refreshWIFILogo)
    {
        this->parentDisplay->fillRect(15, 30, 130, 55, this->sysStats->isWIFIConnected() ? TFT_GREEN : TFT_RED);
        this->parentDisplay->setFont(&fonts::FreeSansBold24pt7b);
        this->parentDisplay->setTextSize(1);
        this->parentDisplay->setTextColor(TFT_WHITE);
        this->parentDisplay->setCursor(25, 40);
        this->parentDisplay->print("WIFI");
        this->refreshWIFILogo = false;
    }
    if (this->refreshWIFISignalStrength)
    {
        this->parentDisplay->setFont(&fonts::FreeMono9pt7b);
        this->parentDisplay->setTextSize(1);
        this->parentDisplay->setCursor(165, 40);
        this->parentDisplay->setTextColor(TFT_WHITE, TFT_BLACK);
        this->parentDisplay->printf("%+03ddBm", this->sysStats->getWIFISignalStrength());
    }
    if (this->refreshWIFISignalLevelGraph)
    {
        int colors[5];
        if (this->sysStats->isWIFIConnected())
        {
            if (this->sysStats->getWIFISignalStrength() < -90)
            {
                colors[0] = TFT_DARKGREY;
                colors[1] = TFT_DARKGREY;
                colors[2] = TFT_DARKGREY;
                colors[3] = TFT_DARKGREY;
                colors[4] = TFT_DARKGREY;
            }
            else if (this->sysStats->getWIFISignalStrength() < -80)
            {
                colors[0] = TFT_GREEN;
                colors[1] = TFT_DARKGREY;
                colors[2] = TFT_DARKGREY;
                colors[3] = TFT_DARKGREY;
                colors[4] = TFT_DARKGREY;
            }
            else if (this->sysStats->getWIFISignalStrength() < -70)
            {
                colors[0] = TFT_GREEN;
                colors[1] = TFT_GREEN;
                colors[2] = TFT_DARKGREY;
                colors[3] = TFT_DARKGREY;
                colors[4] = TFT_DARKGREY;
            }
            else if (this->sysStats->getWIFISignalStrength() < -67)
            {
                colors[0] = TFT_GREEN;
                colors[1] = TFT_GREEN;
                colors[2] = TFT_GREEN;
                colors[3] = TFT_DARKGREY;
                colors[4] = TFT_DARKGREY;
            }
            else if (this->sysStats->getWIFISignalStrength() < -30)
            {
                colors[0] = TFT_GREEN;
                colors[1] = TFT_GREEN;
                colors[2] = TFT_GREEN;
                colors[3] = TFT_GREEN;
                colors[4] = TFT_DARKGREY;
            }
            else
            {
                colors[0] = TFT_GREEN;
                colors[1] = TFT_GREEN;
                colors[2] = TFT_GREEN;
                colors[3] = TFT_GREEN;
                colors[4] = TFT_GREEN;
            }
        }
        else
        {
            colors[0] = TFT_DARKGREY;
            colors[1] = TFT_DARKGREY;
            colors[2] = TFT_DARKGREY;
            colors[3] = TFT_DARKGREY;
            colors[4] = TFT_DARKGREY;
        }
        uint16_t x = 195;
        uint16_t y = 76;
        uint16_t height = 8;
        for (uint8_t i = 0; i < 5; i++)
        {
            this->parentDisplay->fillRect(x, y, 12, height, colors[i]);
            x += 24;
            y -= 12;
            height += 12;
        }
    }
    if (this->refreshWIFIData)
    {
        if (this->sysStats->isWIFIConnected())
        {
            this->parentDisplay->setFont(&fonts::FreeSansBold24pt7b);
            this->parentDisplay->setTextSize(1);
            this->parentDisplay->setTextColor(TFT_WHITE);
            this->parentDisplay->setCursor(12, 100);
            // this->parentDisplay->print("SSID: ");
            //  this->parentDisplay->print(WiFi.SSID());

            char WIFIMacAddress[19];
            this->sysStats->getWIFIMacAddress(WIFIMacAddress, 19);
            char WIFIIPAddress[16];
            this->sysStats->getWIFIIPAddress(WIFIIPAddress, 16);
            this->parentDisplay->setCursor(12, 110);
            this->parentDisplay->print("MAC: ");
            // this->parentDisplay->print(WIFIMacAddress);
            this->parentDisplay->setCursor(12, 110);
            // this->parentDisplay->print("IP: ");
            this->parentDisplay->print(WIFIIPAddress);
        }
        else
        {
            this->parentDisplay->setFont(&fonts::FreeSansBold24pt7b);
            this->parentDisplay->setTextSize(1);
            this->parentDisplay->setTextColor(TFT_WHITE);
            this->parentDisplay->setCursor(12, 100);
            // this->parentDisplay->print("SSID: ");
            //  this->parentDisplay->print(WiFi.SSID());

            char WIFIMacAddress[19];
            this->sysStats->getWIFIMacAddress(WIFIMacAddress, 19);
            char WIFIIPAddress[16];
            this->sysStats->getWIFIIPAddress(WIFIIPAddress, 16);
            this->parentDisplay->setCursor(12, 110);
            this->parentDisplay->print("MAC: ");
            // this->parentDisplay->print(WIFIMacAddress);
            this->parentDisplay->setCursor(12, 110);
            this->parentDisplay->print("IP: ");
            // this->parentDisplay->print(WIFIIPAddress);
        }
        this->refreshWIFIData = false;
    }
    char str[100];
    FormatUtils::millisToString(sysStats->getRuntime(), str, 100);
    this->parentDisplay->setTextSize(1);
    this->parentDisplay->setCursor(12, 220);
    this->parentDisplay->setTextColor(TFT_WHITE, TFT_BLACK);
    this->parentDisplay->printf("FPS: %03u Runtime: %s", this->fpsDebug->getFPS(), str);
}
