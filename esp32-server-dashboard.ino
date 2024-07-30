// #include <Preferences.h>

#define DISPLAY_DRIVER_LOVYANN_ST7789 // at this time, only LovyAnn ST7789 (320x240) driver is supported

#ifdef DISPLAY_DRIVER_LOVYANN_ST7789

#include "src/display/SizesAndOffsets-320x240.hpp"
#include "src/display/ST7789-SPI-240x320-Lovyann/LGFX.hpp"

// these are my custom/valid pin values for a ST7789 display on some esp32 models

// #define ESP32_WROOM
#define ESP32_C3_SUPER_MINI

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

void onReceivedSerialCommand(SerialCommandType cmd, const char *value)
{
    char str[1024] = {'\0'};
    switch (cmd)
    {
    case SERIAL_CMDT_REBOOT:
        Serial.println("Serial command received: rebooting");
        ESP.restart();
        break;
    case SERIAL_CMDT_CLEAR_SETTINGS:
        Serial.println("Serial command received: reset settings");
        settings->clear();
        Serial.println("Settings cleared. Reboot REQUIRED");
        break;
    case SERIAL_CMDT_EXPORT_SETTINGS:
        Serial.println("Serial command received: export settings");
        Serial.println("# EXPORTED SETTINGS BEGIN");
        Serial.println(SerialCommandStr[SERIAL_CMDT_CLEAR_SETTINGS]);
        settings->getWIFISSID(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(SerialCommandStr[SERIAL_CMDT_SET_WIFI_SSID]);
            Serial.println(str);
        }
        settings->getWIFIPassword(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(SerialCommandStr[SERIAL_CMDT_SET_WIFI_PASSWORD]);
            Serial.println(str);
        }
        settings->getMQTTTelegrafURI(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(SerialCommandStr[SERIAL_CMDT_SET_MQTT_TELEGRAF_URI]);
            Serial.println(str);
        }
        settings->getMQTTTelegrafGlobalTopic(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(SerialCommandStr[SERIAL_CMDT_SET_MQTT_TELEGRAF_TOPIC]);
            Serial.println(str);
        }
        Serial.println("REBOOT");
        Serial.println("# EXPORTED SETTINGS END");
        break;
    case SERIAL_CMDT_CONNECT_WIFI:
        Serial.println("Serial command received: connect WiFi");
        WifiManager::connect(false);
        break;
    case SERIAL_CMDT_DISCONNECT_WIFI:
        Serial.println("Serial command received: disconnect WiFi");
        WifiManager::disconnect();
        break;
    case SERIAL_CMDT_SET_WIFI_SSID:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set WiFi SSID (%s)\n", value);
            settings->setWIFISSID(value);
            Serial.println("WiFi SSID saved. Reboot REQUIRED");
        }
        else
        {
            Serial.println("Serial command received: unset WiFi SSID");
            settings->setWIFISSID("");
            Serial.println("WiFi SSID removed. Reboot REQUIRED");
        }
        break;
    case SERIAL_CMDT_SET_WIFI_PASSWORD:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set WiFi password (%s)\n", value);
            settings->setWIFIPassword(value);
            Serial.println("WiFi password saved. Reboot REQUIRED");
        }
        else
        {
            Serial.println("Serial command received: unset WiFi password");
            settings->setWIFIPassword("");
            Serial.println("WiFi SSID removed. Reboot REQUIRED");
        }
        break;
    case SERIAL_CMDT_SET_MQTT_TELEGRAF_URI:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set MQTT Telegraf URI (%s)\n", value);
            settings->setMQTTTelegrafURI(value);
            Serial.println("MQTT Telegraf URI saved. Reboot REQUIRED");
        }
        else
        {
            Serial.println("Serial command received: unset MQTT Telegraf URI");
            settings->setMQTTTelegrafURI("");
            Serial.println("MQTT Telegraf URI removed. Reboot REQUIRED");
        }
        break;
    case SERIAL_CMDT_SET_MQTT_TELEGRAF_TOPIC:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set MQTT Telegraf global topic (%s)\n", value);
            settings->setMQTTTelegrafGlobalTopic(value);
            Serial.println("MQTT Telegraf global topic saved. Reboot REQUIRED");
        }
        else
        {
            Serial.println("Serial command received: unset MQTT Telegraf global topic");
            settings->setMQTTTelegrafGlobalTopic("");
            Serial.println("MQTT Telegraf global topic removed. Reboot REQUIRED");
        }
        break;
    case SERIAL_CMDT_SET_SCREEN:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: change screen to (%s)\n", value);
            if (strcmp(value, "INFO") == 0)
            {
                if (screen->flipToScreen(ST_INFO))
                {
                    Serial.println("Screen changed");
                }
                else
                {
                    Serial.println("Screen unchanged");
                }
            }
            else if (strcmp(value, "DATA_RESUME") == 0)
            {
                if (screen->flipToScreen(ST_DATA_RESUME))
                {
                    Serial.println("Screen changed");
                }
                else
                {
                    Serial.println("Screen unchanged");
                }
            }
            else
            {
                Serial.println("Invalid screen");
            }
        }
        break;
    default:
        Serial.println("Serial command received (UNKNOWN):");
        if (value)
        {
            Serial.println(value);
        }
        break;
    }
}

void setup()
{
    //  TODO: default info screen if no valid settings found
    //  TODO: rotary encoder controller, button pressed at boot = enter settings mode, movement = toggle between screens

    SerialManager::init(SerialManager::DEFAULT_SPEED, onReceivedSerialCommand);
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
        // mqttTelegrafSRC = new MQTTTelegrafSource(mqttTelegrafURI, WiFiMacAddress, mqttTelegrafGlobalTopic);
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
