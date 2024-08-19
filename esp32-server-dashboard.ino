#include <Arduino.h>
#include <Bounce2.h>
#include <cstdint>

#define DISPLAY_DRIVER_LOVYANN_ST7789 // at this time, only LovyAnn ST7789 (320x240) driver is supported

#ifdef DISPLAY_DRIVER_LOVYANN_ST7789

#include "src/display/SizesAndOffsets-320x240.hpp"
#include "src/display/ST7789-SPI-240x320-Lovyann/LGFX.hpp"
#include "src/display/ScreenType.hpp"

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

#define PIN_BUTTON_SW 10

#else
#ifdef ESP32_WROOM
// ESP32 WROOM custom pins
#define PIN_CS 5
#define PIN_RST 4
#define PIN_DC 2
#define PIN_SDA 23
#define PIN_SCL 18

#error TODO_ENCODER_PINS

#else
#error UNSUPPORTED_MODEL
#endif // ESP32_WROOM
#endif // ESP32_C3_SUPER_MINI

LGFX *screen = nullptr;

#else
#error NO_DISPLAY_DRIVER
#endif // DISPLAY_DRIVER_LOVYANN_ST7789

#define SOURCE_DUMMY // this is for testing source with random values (debug)
// #define SOURCE_MQTT_TELEGRAF // this is telegraf source via mqtt protocol (production)

#include "src/utils/Settings.hpp"
#include "src/utils/WifiManager.hpp"
#include "src/utils/SerialManager.hpp"
#include "src/utils/Format.hpp"
#include "src/utils/FPS.hpp"
#include "src/sources/SourceData.hpp"
#ifdef SOURCE_DUMMY
#define SOURCE_DUMMY_UPDATES_EVERY_MS 0 // random data will be refreshed (on method refresh call) every nn milliseconds (0 = no delay)
#include "src/sources/dummy/DummySource.hpp"
#else
#ifdef SOURCE_MQTT_TELEGRAF
#include "src/sources/mqtt/MQTTTelegrafSource.hpp"
#endif // SOURCE_MQTT_TELEGRAF
#endif // SOURCE_DUMMY

#ifdef SOURCE_DUMMY
DummySource *dummySRC = nullptr;
#else
#ifdef SOURCE_MQTT_TELEGRAF
MQTTTelegrafSource *mqttTelegrafSRC = nullptr;
#endif // SOURCE_MQTT_TELEGRAF
#endif // SOURCE_DUMMY

SourceData *sourceData = nullptr;
Bounce2::Button *button;

// create / delete telegraf/mqtt handler on wifi connect/disconnect events
void onWifiConnectionChanged(bool connected)
{
    if (connected)
    {
#ifdef SOURCE_MQTT_TELEGRAF
        char mqttTelegrafURI[64] = {'\0'};
        Settings::getMQTTTelegrafURI(mqttTelegrafURI, sizeof(mqttTelegrafURI));
        char mqttTelegrafGlobalTopic[512] = {'\0'};
        Settings::getMQTTTelegrafGlobalTopic(mqttTelegrafGlobalTopic, sizeof(mqttTelegrafGlobalTopic));
        char networkInterfaceId[MAX_NETWORK_INTERFACE_ID_LENGTH];
        Settings::getNetworkInterfaceId(networkInterfaceId, sizeof(networkInterfaceId));
        if (strlen(mqttTelegrafURI) > 0 && strlen(mqttTelegrafGlobalTopic) > 0)
        {
            char WiFiMacAddress[32] = {'\0'};
            WifiManager::getMacAddress(WiFiMacAddress, sizeof(WiFiMacAddress));
            mqttTelegrafSRC = new MQTTTelegrafSource(sourceData, mqttTelegrafURI, WiFiMacAddress, mqttTelegrafGlobalTopic, networkInterfaceId);
        }
#endif // SOURCE_MQTT_TELEGRAF
    }
    else
    {
#ifdef SOURCE_MQTT_TELEGRAF
        if (mqttTelegrafSRC != nullptr)
        {
            delete mqttTelegrafSRC;
            mqttTelegrafSRC = nullptr;
        }
#endif // SOURCE_MQTT_TELEGRAF
    }
}

void onReceivedSerialCommand(SerialCommandType cmd, const char *value)
{
    char str[1024] = {'\0'};
    uint64_t tmpUint64 = 0;
    float tmpFloat = 0.0f;
    switch (cmd)
    {
    case SERIAL_CMDT_REBOOT:
        Serial.println("Serial command received: rebooting");
        ESP.restart();
        break;
    case SERIAL_CMDT_CLEAR_SETTINGS:
        Serial.println("Serial command received: reset settings");
        Settings::clear();
        Serial.println("Settings cleared. Reboot REQUIRED");
        break;
    case SERIAL_CMDT_EXPORT_SETTINGS:
        Serial.println("Serial command received: export settings");
        Serial.println("# EXPORTED SETTINGS BEGIN");
        Serial.println(SerialCommandStr[SERIAL_CMDT_CLEAR_SETTINGS]);
        Settings::getWIFISSID(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(SerialCommandStr[SERIAL_CMDT_SET_WIFI_SSID]);
            Serial.println(str);
        }
        Settings::getWIFIPassword(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(SerialCommandStr[SERIAL_CMDT_SET_WIFI_PASSWORD]);
            Serial.println(str);
        }
        Settings::getMQTTTelegrafURI(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(SerialCommandStr[SERIAL_CMDT_SET_MQTT_TELEGRAF_URI]);
            Serial.println(str);
        }
        Settings::getMQTTTelegrafGlobalTopic(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(SerialCommandStr[SERIAL_CMDT_SET_MQTT_TELEGRAF_TOPIC]);
            Serial.println(str);
        }
        tmpUint64 = Settings::getMaxDownloadBandwidthBytes();
        if (tmpUint64 > 0)
        {
            Serial.print(SerialCommandStr[SERIAL_CMDT_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH]);
            Serial.println(tmpUint64);
        }
        tmpUint64 = Settings::getMaxUploadBandwidthBytes();
        if (tmpUint64 > 0)
        {
            Serial.print(SerialCommandStr[SERIAL_CMDT_SET_MAX_UPLOAD_BYTES_BANDWIDTH]);
            Serial.println(tmpUint64);
        }
        Settings::getNetworkInterfaceId(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(SerialCommandStr[SERIAL_CMDT_SET_NETWORK_INTERFACE_ID]);
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
            if (Settings::setWIFISSID(value))
            {
                Serial.println("WiFi SSID saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error saving WiFi SSID");
            }
        }
        else
        {
            Serial.println("Serial command received: unset WiFi SSID");
            if (Settings::setWIFISSID(""))
            {
                Serial.println("WiFi SSID removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing WiFi SSID");
            }
        }
        break;
    case SERIAL_CMDT_SET_WIFI_PASSWORD:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set WiFi password (%s)\n", value);
            if (Settings::setWIFIPassword(value))
            {
                Serial.println("WiFi password saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error saving WiFi password");
            }
        }
        else
        {
            Serial.println("Serial command received: unset WiFi password");
            if (Settings::setWIFIPassword(""))
            {
                Serial.println("WiFi SSID removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing WiFi password");
            }
        }
        break;
    case SERIAL_CMDT_SET_MQTT_TELEGRAF_URI:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set MQTT Telegraf URI (%s)\n", value);
            if (Settings::setMQTTTelegrafURI(value))
            {
                Serial.println("MQTT Telegraf URI saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error saving MQTT Telegraf URI");
            }
        }
        else
        {
            Serial.println("Serial command received: unset MQTT Telegraf URI");
            if (Settings::setMQTTTelegrafURI(""))
            {
                Serial.println("MQTT Telegraf URI removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing MQTT Telegraf URI");
            }
        }
        break;
    case SERIAL_CMDT_SET_MQTT_TELEGRAF_TOPIC:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set MQTT Telegraf global topic (%s)\n", value);
            if (Settings::setMQTTTelegrafGlobalTopic(value))
            {
                Serial.println("MQTT Telegraf global topic saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error saving MQTT Telegraf global topic");
            }
        }
        else
        {
            Serial.println("Serial command received: unset MQTT Telegraf global topic");
            if (Settings::setMQTTTelegrafGlobalTopic(""))
            {
                Serial.println("MQTT Telegraf global topic removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing MQTT Telegraf global topic");
            }
        }
        break;
    case SERIAL_CMDT_TOGGLE_SCREEN:
        if (screen != nullptr)
        {
            screen->toggleScreen();
        }
        break;
    case SERIAL_CMDT_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set max download bandwidth bytes (%s)\n", value);
            if (Settings::setMaxDownloadBandwidthBytes(strtoull(value, nullptr, 10)))
            {
                Serial.println("Max download bandwidth bytes saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error saving max download bandwidth bytes");
            }
        }
        else
        {
            Serial.println("Serial command received: unset max download bandwidth bytes");
            if (Settings::setMaxDownloadBandwidthBytes(0))
            {
                Serial.println("Max download bandwidth bytes removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing max download bandwidth bytes");
            }
        }
        break;
    case SERIAL_CMDT_SET_MAX_UPLOAD_BYTES_BANDWIDTH:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set max upload bandwidth bytes (%s)\n", value);
            if (Settings::setMaxUploadBandwidthBytes(strtoull(value, nullptr, 10)))
            {
                Serial.println("Max upload bandwidth bytes saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error saving max upload bandwidth bytes");
            }
        }
        else
        {
            Serial.println("Serial command received: unset max upload bandwidth bytes");
            if (Settings::setMaxUploadBandwidthBytes(0))
            {
                Serial.println("Max upload bandwidth bytes removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing max upload bandwidth bytes");
            }
        }
        break;
    case SERIAL_CMDT_SET_NETWORK_INTERFACE_ID:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set network interface id (%s)\n", value);
            if (Settings::setNetworkInterfaceId(value))
            {
                Serial.println("Network interface id saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error saving network interface id");
            }
        }
        else
        {
            Serial.println("Serial command received: unset network interface id");
            if (Settings::setNetworkInterfaceId(""))
            {
                Serial.println("Network interface id removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing network interface id");
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

    char WiFiSSID[WIFI_SSID_CHAR_ARR_LENGTH];
    Settings::getWIFISSID(WiFiSSID, WIFI_SSID_CHAR_ARR_LENGTH);

    char WiFiPassword[WIFI_PASSWORD_CHAR_ARR_LENGTH];
    Settings::getWIFIPassword(WiFiPassword, WIFI_PASSWORD_CHAR_ARR_LENGTH);

    WifiManager::setConnectionCallbackHandler(onWifiConnectionChanged);
    WifiManager::setCredentials(WiFiSSID, WiFiPassword);
    WifiManager::connect(true);

    sourceData = new SourceData(true, Settings::getMaxDownloadBandwidthBytes(), Settings::getMaxUploadBandwidthBytes());
#ifdef SOURCE_DUMMY
    dummySRC = new DummySource(sourceData);
#endif // SOURCE_DUMMY
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    screen = new LGFX(PIN_SDA, PIN_SCL, PIN_CS, PIN_DC, PIN_RST, DISPLAY_DRIVER_LOVYANN_ST7789_WIDTH, DISPLAY_DRIVER_LOVYANN_ST7789_HEIGHT, DISPLAY_DRIVER_LOVYANN_ST7789_ROTATION);
    screen->setSourceData(sourceData);
    screen->initScreen(ST_INFO);
    // screen->initScreen(ST_DATA_RESUME);
#endif // DISPLAY_DRIVER_LOVYANN_ST7789
    button = new Bounce2::Button();
    button->attach(PIN_BUTTON_SW, INPUT_PULLUP);
    button->interval(5);
    button->setPressedState(LOW);
}

void loop()
{
    SerialManager::loop();
    WifiManager::loop();
#ifdef SOURCE_DUMMY
    dummySRC->refresh(SOURCE_DUMMY_UPDATES_EVERY_MS);
#endif // SOURCE_DUMMY
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    button->update();
    if (button->pressed())
    {
        screen->toggleScreen();
    }
    screen->refresh();
    FPS::loop(999);
#endif // DISPLAY_DRIVER_LOVYANN_ST7789
    yield();
}
