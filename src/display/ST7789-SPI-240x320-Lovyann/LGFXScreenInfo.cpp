#include "LGFXScreenInfo.hpp"
#include "../../utils/Format.hpp"
#include "../../utils/SerialManager.hpp"
#include "../../utils/FPS.hpp"
#include "../../CommonDefines.hpp"
#include <cstring>

#define SCREEN_TITLE_TEXT "ESP32-SERVER-DASHBBOARD"
#define SCREEN_TITLE_TEXT_COLOR TFT_WHITE
#define SCREEN_TITLE_TEXT_BG_COLOR TFT_BLACK
#define SCREEN_TITLE_FONT &fonts::Font0
#define SCREEN_TITLE_FONT_SIZE 2
#define SCREEN_TITLE_X_OFFSET 22
#define SCREEN_TITLE_Y_OFFSET 4

#define SCREEN_WIFI_LOGO_RECT_X_OFFSET 18
#define SCREEN_WIFI_LOGO_RECT_Y_OFFSET 30
#define SCREEN_WIFI_LOGO_RECT_WIDTH 130
#define SCREEN_WIFI_LOGO_RECT_HEIGHT 55
#define SCREEN_WIFI_LOGO_BG_COLOR_CONNECTED TFT_GREEN
#define SCREEN_WIFI_LOGO_BG_COLOR_DISCONNECTED TFT_RED
#define SCREEN_WIFI_LOGO_FONT &fonts::FreeSansBold24pt7b
#define SCREEN_WIFI_LOGO_FONT_SIZE 1
#define SCREEN_WIFI_LOGO_TEXT_COLOR TFT_WHITE
#define SCREEN_WIFI_LOGO_TEXT_X_OFFSET 28
#define SCREEN_WIFI_LOGO_TEXT_Y_OFFSET 40
#define SCREEN_WIFI_LOGO_TEXT "WIFI"

#define SCREEN_WIFI_SIGNAL_STRENGTH_FONT &fonts::FreeMono9pt7b
#define SCREEN_WIFI_SIGNAL_STRENGTH_FONT_SIZE 1
#define SCREEN_WIFI_SIGNAL_STRENGTH_TEXT_COLOR TFT_WHITE
#define SCREEN_WIFI_SIGNAL_STRENGTH_TEXT_BG_COLOR TFT_BLACK
#define SCREEN_WIFI_SIGNAL_STRENGTH_TEXT_X_OFFSET 172
#define SCREEN_WIFI_SIGNAL_STRENGTH_TEXT_Y_OFFSET 36

#define TOTAL_WIFI_SIGNAL_LEVEL_BARS 5
#define WIFI_SIGNAL_LEVEL_BARS_START_X_OFFSET 195
#define WIFI_SIGNAL_LEVEL_BARS_START_Y_OFFSET 76
#define WIFI_SIGNAL_LEVEL_BARS_START_MIN_HEIGHT 8
#define WIFI_SIGNAL_LEVEL_BARS_HEIGHT_INCREMENT 12
#define WIFI_SIGNAL_LEVEL_BARS_WIDTH 12
#define WIFI_SIGNAL_LEVEL_BARS_NEXT_X_OFFSET 24

#define SCREEN_COMMON_TEXTDATA_FONT &fonts::FreeMono9pt7b
#define SCREEN_COMMON_TEXTDATA_FONT_SIZE 1
#define SCREEN_COMMON_TEXTDATA_COLOR TFT_WHITE
#define SCREEN_COMMON_TEXTDATA_BG_COLOR TFT_BLACK

#define SCREEN_COMMON_TEXTDATA_FIELD_X_OFFSET 5
#define SCREEN_COMMON_TEXTDATA_FIELD_VALUE_X_OFFSET 60

LGFXScreenInfo::LGFXScreenInfo(LovyanGFX *display) : LGFXScreen(display)
{
    if (display != nullptr)
    {
        FPS::Init();
        // top title
        this->parentDisplay->setFont(SCREEN_TITLE_FONT);
        this->parentDisplay->setTextSize(SCREEN_TITLE_FONT_SIZE);
        this->parentDisplay->setTextColor(SCREEN_TITLE_TEXT_COLOR, SCREEN_TITLE_TEXT_BG_COLOR);
        this->parentDisplay->setCursor(SCREEN_TITLE_X_OFFSET, SCREEN_TITLE_Y_OFFSET);
        this->parentDisplay->println(SCREEN_TITLE_TEXT);

        this->wasConnected = WiFiManager::IsConnected();
        this->refresh(true);
    }
}

LGFXScreenInfo::~LGFXScreenInfo()
{
    this->parentDisplay = nullptr;
}

void LGFXScreenInfo::refreshWIFILogo(void)
{
    this->parentDisplay->fillRect(SCREEN_WIFI_LOGO_RECT_X_OFFSET, SCREEN_WIFI_LOGO_RECT_Y_OFFSET, SCREEN_WIFI_LOGO_RECT_WIDTH, SCREEN_WIFI_LOGO_RECT_HEIGHT, WiFiManager::IsConnected() ? SCREEN_WIFI_LOGO_BG_COLOR_CONNECTED : SCREEN_WIFI_LOGO_BG_COLOR_DISCONNECTED);
    this->parentDisplay->setFont(SCREEN_WIFI_LOGO_FONT);
    this->parentDisplay->setTextSize(SCREEN_WIFI_LOGO_FONT_SIZE);
    this->parentDisplay->setTextColor(SCREEN_WIFI_LOGO_TEXT_COLOR);
    this->parentDisplay->setCursor(SCREEN_WIFI_LOGO_TEXT_X_OFFSET, SCREEN_WIFI_LOGO_TEXT_Y_OFFSET);
    this->parentDisplay->print(SCREEN_WIFI_LOGO_TEXT);
}

void LGFXScreenInfo::refreshWIFISignalStrength(void)
{
    this->parentDisplay->setFont(SCREEN_WIFI_SIGNAL_STRENGTH_FONT);
    this->parentDisplay->setTextSize(SCREEN_WIFI_SIGNAL_STRENGTH_FONT_SIZE);
    this->parentDisplay->setTextColor(SCREEN_WIFI_SIGNAL_STRENGTH_TEXT_COLOR, SCREEN_WIFI_SIGNAL_STRENGTH_TEXT_BG_COLOR);
    this->parentDisplay->setCursor(SCREEN_WIFI_SIGNAL_STRENGTH_TEXT_X_OFFSET, SCREEN_WIFI_SIGNAL_STRENGTH_TEXT_Y_OFFSET);
    if (WiFiManager::IsConnected())
    {
        this->parentDisplay->printf("%+02ddBm", WiFiManager::GetSignalStrength());
    }
    else
    {
        this->parentDisplay->printf("      ", WiFiManager::GetSignalStrength());
    }
}

void LGFXScreenInfo::refreshWIFISignalLevelBars(void)
{
    int colors[TOTAL_WIFI_SIGNAL_LEVEL_BARS];
    if (WiFiManager::IsConnected())
    {
        switch (WiFiManager::ConvertToSignalQuality(WiFiManager::GetSignalStrength()))
        {
        case WIFI_SIGNAL_QUALITY_NONE:
            colors[0] = TFT_DARKGREY;
            colors[1] = TFT_DARKGREY;
            colors[2] = TFT_DARKGREY;
            colors[3] = TFT_DARKGREY;
            colors[4] = TFT_DARKGREY;
            break;
        case WIFI_SIGNAL_QUALITY_WORST:
            colors[0] = TFT_GREEN;
            colors[1] = TFT_DARKGREY;
            colors[2] = TFT_DARKGREY;
            colors[3] = TFT_DARKGREY;
            colors[4] = TFT_DARKGREY;
            break;
        case WIFI_SIGNAL_QUALITY_BAD:
            colors[0] = TFT_GREEN;
            colors[1] = TFT_GREEN;
            colors[2] = TFT_DARKGREY;
            colors[3] = TFT_DARKGREY;
            colors[4] = TFT_DARKGREY;
            break;
        case WIFI_SIGNAL_QUALITY_NORMAL:

            colors[0] = TFT_GREEN;
            colors[1] = TFT_GREEN;
            colors[2] = TFT_GREEN;
            colors[3] = TFT_DARKGREY;
            colors[4] = TFT_DARKGREY;
            break;
        case WIFI_SIGNAL_QUALITY_GOOD:
            colors[0] = TFT_GREEN;
            colors[1] = TFT_GREEN;
            colors[2] = TFT_GREEN;
            colors[3] = TFT_GREEN;
            colors[4] = TFT_DARKGREY;
            break;
        case WIFI_SIGNAL_QUALITY_BEST:

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
    uint16_t x = WIFI_SIGNAL_LEVEL_BARS_START_X_OFFSET;
    uint16_t y = WIFI_SIGNAL_LEVEL_BARS_START_Y_OFFSET;
    uint16_t height = WIFI_SIGNAL_LEVEL_BARS_START_MIN_HEIGHT;
    for (uint8_t i = 0; i < TOTAL_WIFI_SIGNAL_LEVEL_BARS; i++)
    {
        this->parentDisplay->fillRect(x, y, WIFI_SIGNAL_LEVEL_BARS_WIDTH, height, colors[i]);
        x += WIFI_SIGNAL_LEVEL_BARS_NEXT_X_OFFSET;
        y -= WIFI_SIGNAL_LEVEL_BARS_HEIGHT_INCREMENT;
        height += WIFI_SIGNAL_LEVEL_BARS_HEIGHT_INCREMENT;
    }
}

void LGFXScreenInfo::refreshWIFIData(bool forceDrawAll)
{
    WiFiManager::GetSSID(this->WIFISSID, sizeof(this->WIFISSID));
    WiFiManager::GetMacAddress(this->WIFIMacAddress, sizeof(this->WIFIMacAddress));
    WiFiManager::GetIPAddress(this->WIFIIPAddress, sizeof(this->WIFIIPAddress));
    this->parentDisplay->setFont(SCREEN_COMMON_TEXTDATA_FONT);
    this->parentDisplay->setTextSize(SCREEN_COMMON_TEXTDATA_FONT_SIZE);
    this->parentDisplay->setTextColor(SCREEN_COMMON_TEXTDATA_COLOR, SCREEN_COMMON_TEXTDATA_BG_COLOR);
    if (forceDrawAll)
    {
        this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_X_OFFSET, 100);
        this->parentDisplay->print("SSID ");
    }
    this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_VALUE_X_OFFSET, 100);
    this->parentDisplay->printf("%s", this->WIFISSID);
    if (forceDrawAll)
    {
        this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_X_OFFSET, 120);
        this->parentDisplay->print(" MAC ");
    }
    this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_VALUE_X_OFFSET, 120);
    this->parentDisplay->print(this->WIFIMacAddress);
    if (forceDrawAll)
    {
        this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_X_OFFSET, 140);
        this->parentDisplay->print("  IP ");
    }
    this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_VALUE_X_OFFSET, 140);
    this->parentDisplay->printf("%s", this->WIFIIPAddress);
}

bool LGFXScreenInfo::refreshCommonData(bool forceDrawAll)
{
    bool changed = forceDrawAll;
    this->parentDisplay->setFont(SCREEN_COMMON_TEXTDATA_FONT);
    this->parentDisplay->setTextSize(SCREEN_COMMON_TEXTDATA_FONT_SIZE);
    this->parentDisplay->setTextColor(SCREEN_COMMON_TEXTDATA_COLOR, SCREEN_COMMON_TEXTDATA_BG_COLOR);
    if (forceDrawAll)
    {
        this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_X_OFFSET, 160);
        this->parentDisplay->printf(" VER %s (%s)", ESP32_SERVER_DASHBOARD_CURRENT_VERSION, ESP32_SERVER_DASHBOARD_CURRENT_VERSION_DATE);
        this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_X_OFFSET, 180);
        this->parentDisplay->printf(" SPD %u bauds", SerialManager::DEFAULT_SPEED);
    }
    if (forceDrawAll)
    {
        this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_X_OFFSET, 200);
        this->parentDisplay->print(" FPS ");
    }
    uint16_t currentFPS = FPS::GetFPS();
    if (forceDrawAll || this->previousFPS != currentFPS)
    {
        this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_VALUE_X_OFFSET, 200);
        this->parentDisplay->printf("%04u", currentFPS);
        changed = true;
    }
    if (forceDrawAll)
    {
        this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_X_OFFSET, 220);
        this->parentDisplay->print(" RUN ");
    }
    char str[sizeof(this->previousRuntimeStr)];
    Format::ParseMillisToHumanString(millis(), str, sizeof(this->previousRuntimeStr));
    if (forceDrawAll || strcmp(str, this->previousRuntimeStr) != 0)
    {
        this->parentDisplay->setCursor(SCREEN_COMMON_TEXTDATA_FIELD_VALUE_X_OFFSET, 220);
        this->parentDisplay->printf("%s    ", str);
        strncpy(this->previousRuntimeStr, str, sizeof(this->previousRuntimeStr));
        changed = true;
    }
    return (changed);
}

bool LGFXScreenInfo::refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        bool changed = force;
        this->WIFILogoChanged = false;
        this->WIFISignalStrengthChanged = false;
        this->WIFISignalLevelBarsChanged = false;
        this->WIFIDataChanged = false;

        bool isConnected = WiFiManager::IsConnected();
        if (!force)
        {
            if (this->wasConnected != isConnected)
            {
                this->WIFILogoChanged = true;
                this->WIFISignalStrengthChanged = true;
                this->WIFISignalLevelBarsChanged = true;
                this->WIFIDataChanged = true;
            }
            else if (isConnected && (strlen(this->WIFISSID) == 0 || strlen(this->WIFIIPAddress) == 0 || strlen(this->WIFIMacAddress) == 0))
            {
                // UGLY: I think there is a "timming" bug that not assign ssid/address on some connected networks so every time that connected status found if no
                // data is assigned try to refresh
                this->WIFIDataChanged = true;
                changed = true;
            }
        }
        long currentWiFiSignalStrength = WiFiManager::GetSignalStrength();
        if (this->previousWiFiSignalStrength != currentWiFiSignalStrength)
        {
            this->WIFISignalStrengthChanged = true;
            changed = true;
        }
        WIFI_SIGNAL_QUALITY currentWIFI_SIGNAL_QUALITY = WiFiManager::ConvertToSignalQuality(currentWiFiSignalStrength);
        if (this->previousWIFI_SIGNAL_QUALITY != currentWIFI_SIGNAL_QUALITY)
        {
            this->WIFISignalLevelBarsChanged = true;
            changed = true;
        }
        this->wasConnected = isConnected;
        this->previousWiFiSignalStrength = currentWiFiSignalStrength;
        this->previousWIFI_SIGNAL_QUALITY = currentWIFI_SIGNAL_QUALITY;

        if (force || this->WIFILogoChanged)
        {
            this->refreshWIFILogo();
        }

        if (force || this->WIFISignalStrengthChanged)
        {
            this->refreshWIFISignalStrength();
        }

        if (force || this->WIFISignalLevelBarsChanged)
        {
            this->refreshWIFISignalLevelBars();
        }

        if (force || this->WIFIDataChanged)
        {
            this->refreshWIFIData(force);
        }

        return (this->refreshCommonData(force) || changed);
    }
    else
    {
        return (false);
    }
}
