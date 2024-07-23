#include "LGFXScreenInfo.hpp"
#include "../../utils/Format.hpp"

#define DEFAULT_FONT &fonts::Font0
#define CUSTOM_FONT &fonts::FreeMono9pt7b
#define BIG_FONT &fonts::FreeSansBold24pt7b
#define MAIN_SPRITE_WIDTH 312
#define MAIN_SPRITE_HEIGHT 216

LGFXScreenInfo::LGFXScreenInfo(LovyanGFX *display, FPSDebug *fpsDebug) : LGFXScreen(display, fpsDebug)
{
    // this->parentDisplay->drawRect(0, 0, 320, 240, TFT_WHITE); // this is for screen bounds debugging purposes only
    this->parentDisplay->setTextWrap(false);
    this->parentDisplay->setFont(DEFAULT_FONT);
    this->parentDisplay->setTextSize(2);
    this->parentDisplay->setTextColor(TFT_WHITE);
    this->parentDisplay->setCursor(25, 4);
    this->parentDisplay->println("ESP32-SERVER-DASHBBOARD");

    this->refreshWIFILogo = true;
    this->refreshWIFISignalStrength = true;
    this->refreshWIFISignalLevelGraph = true;
    this->refreshWIFIData = true;
    this->refresh(true);
}

LGFXScreenInfo::~LGFXScreenInfo()
{
}

void LGFXScreenInfo::refresh(bool firstRun)
{
    this->fpsDebug->loop();

    if (!firstRun)
    {
        bool isConnected = WifiManager::isConnected();
        if (this->wasConnected != isConnected)
        {
            this->refreshWIFILogo = true;
            this->refreshWIFISignalStrength = true;
            this->refreshWIFISignalLevelGraph = true;
            this->refreshWIFIData = true;
        }
        long currentSignalStrength = WifiManager::getSignalStrength();
        if (this->previousSignalStrength != currentSignalStrength)
        {
            this->refreshWIFISignalStrength = true;
        }
        WIFISignalQuality currentSignalQuality = WifiManager::getSignalQuality(currentSignalStrength);
        if (this->previousSignalQuality != currentSignalQuality)
        {
            // TODO: not refreshing on disconnect
            this->refreshWIFISignalLevelGraph = true;
        }
        this->wasConnected = isConnected;
        this->previousSignalStrength = currentSignalStrength;
        this->previousSignalQuality = currentSignalQuality;
    }
    else
    {
        this->wasConnected = WifiManager::isConnected();
    }
    if (firstRun || this->refreshWIFILogo)
    {
        this->parentDisplay->fillRect(18, 30, 130, 55, WifiManager::isConnected() ? TFT_GREEN : TFT_RED);
        this->parentDisplay->setFont(BIG_FONT);
        this->parentDisplay->setTextSize(1);
        this->parentDisplay->setTextColor(TFT_WHITE);
        this->parentDisplay->setCursor(28, 40);
        this->parentDisplay->print("WIFI");
        this->refreshWIFILogo = false;
    }
    if (firstRun || this->refreshWIFISignalStrength)
    {
        this->parentDisplay->setFont(CUSTOM_FONT);
        this->parentDisplay->setTextSize(1);
        this->parentDisplay->setTextColor(TFT_WHITE, TFT_BLACK);
        this->parentDisplay->setCursor(172, 36);
        if (WifiManager::isConnected())
        {
            this->parentDisplay->printf("%+02ddBm", WifiManager::getSignalStrength());
        }
        else
        {
            this->parentDisplay->printf("      ", WifiManager::getSignalStrength());
        }
    }
    if (firstRun || this->refreshWIFISignalLevelGraph)
    {
        int colors[5];
        if (WifiManager::isConnected())
        {
            switch (WifiManager::getSignalQuality(WifiManager::getSignalStrength()))
            {
            case WIFISignalQuality_NONE:
                colors[0] = TFT_DARKGREY;
                colors[1] = TFT_DARKGREY;
                colors[2] = TFT_DARKGREY;
                colors[3] = TFT_DARKGREY;
                colors[4] = TFT_DARKGREY;
                break;
            case WIFISignalQuality_WORST:
                colors[0] = TFT_GREEN;
                colors[1] = TFT_DARKGREY;
                colors[2] = TFT_DARKGREY;
                colors[3] = TFT_DARKGREY;
                colors[4] = TFT_DARKGREY;
                break;
            case WIFISignalQuality_BAD:
                colors[0] = TFT_GREEN;
                colors[1] = TFT_GREEN;
                colors[2] = TFT_DARKGREY;
                colors[3] = TFT_DARKGREY;
                colors[4] = TFT_DARKGREY;
                break;
            case WIFISignalQuality_NORMAL:

                colors[0] = TFT_GREEN;
                colors[1] = TFT_GREEN;
                colors[2] = TFT_GREEN;
                colors[3] = TFT_DARKGREY;
                colors[4] = TFT_DARKGREY;
                break;
            case WIFISignalQuality_GOOD:
                colors[0] = TFT_GREEN;
                colors[1] = TFT_GREEN;
                colors[2] = TFT_GREEN;
                colors[3] = TFT_GREEN;
                colors[4] = TFT_DARKGREY;
                break;
            case WIFISignalQuality_BEST:

                colors[0] = TFT_GREEN;
                colors[1] = TFT_GREEN;
                colors[2] = TFT_GREEN;
                colors[3] = TFT_GREEN;
                colors[4] = TFT_GREEN;
                break;
            default:
                colors[0] = TFT_DARKGREY;
                colors[1] = TFT_DARKGREY;
                colors[2] = TFT_DARKGREY;
                colors[3] = TFT_DARKGREY;
                colors[4] = TFT_DARKGREY;
                break;
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
    if (firstRun || this->refreshWIFIData)
    {
        this->parentDisplay->setFont(CUSTOM_FONT);
        this->parentDisplay->setTextSize(1);
        this->parentDisplay->setTextColor(TFT_WHITE, TFT_BLACK);
        this->parentDisplay->setCursor(10, 100);
        this->parentDisplay->print("SSID ");
        // TODO: clear if changed (because str length)
        char WIFISSID[33] = {'\0'};
        WifiManager::getSSID(WIFISSID, sizeof(WIFISSID));
        this->parentDisplay->println(WIFISSID);
        char WIFIMacAddress[19] = {'\0'};
        WifiManager::getMacAddress(WIFIMacAddress, sizeof(WIFIMacAddress));
        char WIFIIPAddress[16] = {'\0'};
        WifiManager::getIPAddress(WIFIIPAddress, sizeof(WIFIIPAddress));
        this->parentDisplay->setCursor(20, 120);
        this->parentDisplay->print("MAC ");
        this->parentDisplay->println(WIFIMacAddress);
        this->parentDisplay->setCursor(30, 140);
        this->parentDisplay->print("IP ");
        this->parentDisplay->println(WIFIIPAddress);
        this->refreshWIFIData = false;
    }
    char str[100];
    Format::millisToHumanStr(millis(), str, 100);
    this->parentDisplay->setFont(CUSTOM_FONT);
    this->parentDisplay->setTextSize(1);
    this->parentDisplay->setTextColor(TFT_WHITE, TFT_BLACK);
    this->parentDisplay->setCursor(12, 220);
    this->parentDisplay->printf("%03uFPS - Runtime: %s    ", this->fpsDebug->getFPS(), str);
}
