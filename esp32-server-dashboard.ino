// #include <Preferences.h>

#define DISPLAY_DRIVER_LOVYANN_ST7789 // at this time, only LovyAnn ST7789 (320x240) driver is supported

#ifdef DISPLAY_DRIVER_LOVYANN_ST7789

#include "src/display/SizesAndOffsets-320x240.hpp"
#include "src/display/ST7789-SPI-240x320-Lovyann/LGFX.hpp"

// these are my custom/valid pin values for a ST7789 display on some esp32 models

#define ESP32_WROOM
// #define ESP32_C3_SUPER_MINI

#ifdef ESP32_C3_SUPER_MINI
// ESP32 C3 SUPER MINI
#define PIN_CS 5
#define PIN_RST 9
#define PIN_DC 8
#define PIN_SDA 6
#define PIN_SCL 4
#else
#ifdef ESP32_WROOM
// ESP32 WROOM custom pins
#define PIN_CS 5
#define PIN_RST 4
#define PIN_DC 2
#define PIN_SDA 23
#define PIN_SCL 18
#else
#error UNSUPPORTED_MODEL
#endif // ESP32_WROOM
#endif // ESP32_C3_SUPER_MINI

LGFX *screen = nullptr;

#else
#error NO_DISPLAY_DRIVER
#endif // DISPLAY_DRIVER_LOVYANN_ST7789

#include "src/utils/Settings.hpp"
#include "src/utils/WifiManager.hpp"
#include "src/utils/SerialManager.hpp"
#include "src/sources/dummy/DummySource.hpp"
#include "src/sources/mqtt/MQTTTelegrafSource.hpp"
#include "src/display/ScreenType.hpp"
#include <Preferences.h>

DummySource *dummySRC = nullptr;
MQTTTelegrafSource *mqttTelegrafSRC = nullptr;
Settings *settings = nullptr;

Preferences preferences;
void setup()
{
    //  TODO: default info screen if no valid settings found
    //  TODO: rotary encoder controller, button pressed at boot = enter settings mode, movement = toggle between screens

    SerialManager::init(SerialManager::DEFAULT_SPEED);
    Serial.println("Starting esp32-server-dashboard");

    settings = new Settings();

    char WiFiSSID[WIFI_SSID_CHAR_ARR_LENGTH];
    settings->getWIFISSID(WiFiSSID, WIFI_SSID_CHAR_ARR_LENGTH);

    char WiFiPassword[WIFI_PASSWORD_CHAR_ARR_LENGTH];
    settings->getWIFIPassword(WiFiPassword, WIFI_PASSWORD_CHAR_ARR_LENGTH);

    WifiManager::setCredentials(WiFiSSID, WiFiPassword);
    WifiManager::connect(true);

    dummySRC = new DummySource();

    char mqttTelegrafURI[64] = {'\0'};
    settings->getMQTTTelegrafURI(mqttTelegrafURI, sizeof(mqttTelegrafURI));
    char mqttTelegrafGlobalTopic[512] = {'\0'};
    settings->getMQTTTelegrafGlobalTopic(mqttTelegrafGlobalTopic, sizeof(mqttTelegrafGlobalTopic));
    if (strlen(mqttTelegrafURI) > 0 && strlen(mqttTelegrafGlobalTopic) > 0)
    {
        char WiFiMacAddress[32] = {'\0'};
        WifiManager::getMacAddress(WiFiMacAddress, sizeof(WiFiMacAddress));
        mqttTelegrafSRC = new MQTTTelegrafSource(mqttTelegrafURI, WiFiMacAddress, mqttTelegrafGlobalTopic);
    }

#ifdef DISPLAY_DRIVER_LOVYANN_ST7789

    screen = new LGFX(PIN_SDA, PIN_SCL, PIN_CS, PIN_DC, PIN_RST, DISPLAY_DRIVER_LOVYANN_ST7789_WIDTH, DISPLAY_DRIVER_LOVYANN_ST7789_HEIGHT, DISPLAY_DRIVER_LOVYANN_ST7789_ROTATION);
    screen->setSource(dummySRC);

    screen->initScreen(ST_INFO);
    // screen->initScreen(ST_DATA_RESUME);
#endif // DISPLAY_DRIVER_LOVYANN_ST7789
}

void loop()
{
    SerialManager::loop();
    WifiManager::loop();
    dummySRC->refresh();
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    screen->refresh();
#endif // DISPLAY_DRIVER_LOVYANN_ST7789
}
