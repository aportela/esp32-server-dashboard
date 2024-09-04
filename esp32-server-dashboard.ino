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
bool screenMirrorFlipVertical = false;

#else
#error NO_DISPLAY_DRIVER
#endif // DISPLAY_DRIVER_LOVYANN_ST7789

// #define SOURCE_DUMMY // this is for testing source with random values (debug)
#define SOURCE_MQTT_TELEGRAF // this is telegraf source via mqtt protocol (production)

#include "src/CustomSettings.hpp"
#include "src/utils/WiFiManager.hpp"
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

CustomSettings *settings = nullptr;
SourceData *sourceData = nullptr;
Bounce2::Button *button;

const char *CUSTOM_SERIAL_COMMANDS[]{
    "REBOOT",
    "CLEAR_SETTINGS",
    "EXPORT_SETTINGS",
    "CONNECT_WIFI",
    "DISCONNECT_WIFI",
    "SET_WIFI_SSID ",
    "SET_WIFI_PASSWORD ",
    "SET_MQTT_TELEGRAF_URI ",
    "SET_MQTT_USERNAME ",
    "SET_MQTT_PASSWORD ",
    "SET_MQTT_TELEGRAF_GLOBAL_TOPIC ",
    "TOGGLE_SCREEN",
    "SET_MAX_DOWNLOAD_BYTES_BANDWITH ",
    "SET_MAX_UPLOAD_BYTES_BANDWITH ",
    "SET_NETWORK_INTERFACE_ID ",
    "SET_HOSTNAME ",
    "SET_SCREEN_MIRROR_FLIP_VERTICAL ",
    "SET_DEFAULT_SCREEN ",
};

// this is not required, but make it more "friendly" eval callback uint_8 command index responses
enum CUSTOM_SERIAL_COMMAND_INDEX
{
    CUSTOM_SERIAL_COMMAND_INDEX_UNKNOWN = -1,
    CUSTOM_SERIAL_COMMAND_INDEX_REBOOT = 0,
    CUSTOM_SERIAL_COMMAND_INDEX_CLEAR_SETTINGS = 1,
    CUSTOM_SERIAL_COMMAND_INDEX_EXPORT_SETTINGS = 2,
    CUSTOM_SERIAL_COMMAND_INDEX_CONNECT_WIFI = 3,
    CUSTOM_SERIAL_COMMAND_INDEX_DISCONNECT_WIFI = 4,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_WIFI_SSID = 5,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_WIFI_PASSWORD = 6,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_TELEGRAF_URI = 7,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_USERNAME = 8,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_PASSWORD = 9,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_TELEGRAF_GLOBAL_TOPIC = 10,
    CUSTOM_SERIAL_COMMAND_INDEX_TOGGLE_SCREEN = 11,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_MAX_DOWNLOAD_BYTES_BANDWITH = 12,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_MAX_UPLOAD_BYTES_BANDWITH = 13,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_NETWORK_INTERFACE_ID = 14,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_HOSTNAME = 15,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_SCREEN_MIRROR_FLIP_VERTICAL = 16,
    CUSTOM_SERIAL_COMMAND_INDEX_SET_DEFAULT_SCREEN = 17,
};

#define CUSTOM_SERIAL_COMMAND_COUNT (sizeof(CUSTOM_SERIAL_COMMANDS) / sizeof(CUSTOM_SERIAL_COMMANDS[0]))

using namespace aportela::microcontroller::utils;

// create / delete telegraf/mqtt handler on wifi connect/disconnect events
void onWifiConnectionStatusChanged(bool connected)
{
    if (connected)
    {
#ifdef SOURCE_MQTT_TELEGRAF
        char mqttTelegrafURI[64] = {'\0'};
        settings->getMQTTTelegrafURI(mqttTelegrafURI, sizeof(mqttTelegrafURI));
        char mqttTelegrafGlobalTopic[512] = {'\0'};
        settings->getMQTTTelegrafGlobalTopic(mqttTelegrafGlobalTopic, sizeof(mqttTelegrafGlobalTopic));
        char networkInterfaceId[MAX_NETWORK_INTERFACE_ID_LENGTH];
        settings->getNetworkInterfaceId(networkInterfaceId, sizeof(networkInterfaceId));
        char mqttUsername[256] = {'\0'};
        settings->getMQTTUsername(mqttUsername, sizeof(mqttUsername));
        char mqttPassword[256] = {'\0'};
        settings->getMQTTPassword(mqttPassword, sizeof(mqttPassword));
        if (strlen(mqttTelegrafURI) > 0 && strlen(mqttTelegrafGlobalTopic) > 0)
        {
            char WiFiMacAddress[32] = {'\0'};
            WiFiManager::getMacAddress(WiFiMacAddress, sizeof(WiFiMacAddress));
            mqttTelegrafSRC = new MQTTTelegrafSource(sourceData, mqttTelegrafURI, WiFiMacAddress, mqttTelegrafGlobalTopic, networkInterfaceId, mqttUsername, mqttPassword);
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

void onReceivedSerialCommand(int8_t commandIndex, const char *value)
{
    char str[1024] = {'\0'};
    uint8_t tmpUint8 = 0;
    uint64_t tmpUint64 = 0;
    float tmpFloat = 0.0f;
    switch ((CUSTOM_SERIAL_COMMAND_INDEX)commandIndex)
    {
    case CUSTOM_SERIAL_COMMAND_INDEX_REBOOT:
        Serial.println("Serial command received: rebooting");
        ESP.restart();
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_CLEAR_SETTINGS:
        Serial.println("Serial command received: reset settings");
        settings->clear();
        Serial.println("Settings cleared. Reboot REQUIRED");
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_EXPORT_SETTINGS:
        Serial.println("Serial command received: export settings");
        Serial.println("# EXPORTED SETTINGS BEGIN");
        Serial.println(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_EXPORT_SETTINGS]);
        settings->getWIFISSID(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_WIFI_SSID]);
            Serial.println(str);
        }
        settings->getWIFIPassword(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_WIFI_PASSWORD]);
            Serial.println(str);
        }
        settings->getMQTTTelegrafURI(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_TELEGRAF_URI]);
            Serial.println(str);
        }
        settings->getMQTTUsername(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_USERNAME]);
            Serial.println(str);
        }
        settings->getMQTTPassword(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_PASSWORD]);
            Serial.println(str);
        }
        settings->getMQTTTelegrafGlobalTopic(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_TELEGRAF_GLOBAL_TOPIC]);
            Serial.println(str);
        }
        tmpUint64 = settings->getMaxDownloadBandwidthBytes();
        if (tmpUint64 > 0)
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_MAX_DOWNLOAD_BYTES_BANDWITH]);
            Serial.println(tmpUint64);
        }
        tmpUint64 = settings->getMaxUploadBandwidthBytes();
        if (tmpUint64 > 0)
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_MAX_UPLOAD_BYTES_BANDWITH]);
            Serial.println(tmpUint64);
        }
        settings->getNetworkInterfaceId(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_NETWORK_INTERFACE_ID]);
            Serial.println(str);
        }
        settings->getHostname(str, sizeof(str));
        if (strlen(str) > 0)
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_HOSTNAME]);
            Serial.println(str);
        }
        tmpUint8 = settings->getDefaultScreen();
        if (tmpUint8 > 0)
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_DEFAULT_SCREEN]);
            Serial.println(tmpUint8);
        }
        if (settings->getScreenMirrorFlipVertical())
        {
            Serial.print(CUSTOM_SERIAL_COMMANDS[CUSTOM_SERIAL_COMMAND_INDEX_SET_SCREEN_MIRROR_FLIP_VERTICAL]);
            Serial.println("true");
        }
        Serial.println("REBOOT");
        Serial.println("# EXPORTED SETTINGS END");
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_CONNECT_WIFI:
        Serial.println("Serial command received: connect WiFi");
        WiFiManager::connect(false);
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_DISCONNECT_WIFI:
        Serial.println("Serial command received: disconnect WiFi");
        WiFiManager::disconnect();
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_WIFI_SSID:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set WiFi SSID (%s)\n", value);
            if (settings->setWIFISSID(value))
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
            if (settings->setWIFISSID(""))
            {
                Serial.println("WiFi SSID removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing WiFi SSID");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_WIFI_PASSWORD:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set WiFi password (%s)\n", value);
            if (settings->setWIFIPassword(value))
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
            if (settings->setWIFIPassword(""))
            {
                Serial.println("WiFi SSID removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing WiFi password");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_TELEGRAF_URI:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set MQTT Telegraf URI (%s)\n", value);
            if (settings->setMQTTTelegrafURI(value))
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
            if (settings->setMQTTTelegrafURI(""))
            {
                Serial.println("MQTT Telegraf URI removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing MQTT Telegraf URI");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_USERNAME:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set MQTT username (%s)\n", value);
            if (settings->setMQTTUsername(value))
            {
                Serial.println("MQTT Telegraf username saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error saving MQTT username");
            }
        }
        else
        {
            Serial.println("Serial command received: unset MQTT username");
            if (settings->setMQTTUsername(""))
            {
                Serial.println("MQTT Telegraf username. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing MQTT username");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_PASSWORD:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set MQTT password (%s)\n", value);
            if (settings->setMQTTPassword(value))
            {
                Serial.println("MQTT Telegraf password. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error saving MQTT password");
            }
        }
        else
        {
            Serial.println("Serial command received: unset MQTT password");
            if (settings->setMQTTPassword(""))
            {
                Serial.println("MQTT password removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing MQTT password");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_MQTT_TELEGRAF_GLOBAL_TOPIC:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set MQTT Telegraf global topic (%s)\n", value);
            if (settings->setMQTTTelegrafGlobalTopic(value))
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
            if (settings->setMQTTTelegrafGlobalTopic(""))
            {
                Serial.println("MQTT Telegraf global topic removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing MQTT Telegraf global topic");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_TOGGLE_SCREEN:
        if (screen != nullptr)
        {
            screen->toggleScreen();
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_MAX_DOWNLOAD_BYTES_BANDWITH:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set max download bandwidth bytes (%s)\n", value);
            if (settings->setMaxDownloadBandwidthBytes(strtoull(value, nullptr, 10)))
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
            if (settings->setMaxDownloadBandwidthBytes(0))
            {
                Serial.println("Max download bandwidth bytes removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing max download bandwidth bytes");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_MAX_UPLOAD_BYTES_BANDWITH:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set max upload bandwidth bytes (%s)\n", value);
            if (settings->setMaxUploadBandwidthBytes(strtoull(value, nullptr, 10)))
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
            if (settings->setMaxUploadBandwidthBytes(0))
            {
                Serial.println("Max upload bandwidth bytes removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing max upload bandwidth bytes");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_NETWORK_INTERFACE_ID:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set network interface id (%s)\n", value);
            if (settings->setNetworkInterfaceId(value))
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
            if (settings->setNetworkInterfaceId(""))
            {
                Serial.println("Network interface id removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing network interface id");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_HOSTNAME:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set hostname (%s)\n", value);
            if (strlen(value) < SourceData::MAX_HOSTNAME_LENGTH + 1)
            {
                if (settings->setHostname(value))
                {
                    Serial.println("Hostname saved. Reboot REQUIRED");
                }
                else
                {
                    Serial.println("Error saving hostname");
                }
            }
            else
            {
                Serial.printf("Invalid hostname (max length %d)\n", SourceData::MAX_HOSTNAME_LENGTH);
            }
        }
        else
        {
            Serial.println("Serial command received: unset hostname");
            if (settings->setHostname(""))
            {
                Serial.println("Hostname removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing hostname");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_SCREEN_MIRROR_FLIP_VERTICAL:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set screen mirror flip vertical flag (%s)\n", value);
            if (settings->setScreenMirrorFlipVertical(true))
            {
                Serial.println("Screen mirror flip vertical flag saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error saving screen mirror flip vertical flag");
            }
        }
        else
        {
            Serial.println("Serial command received: unset screen mirror flip vertical flag");
            if (settings->setScreenMirrorFlipVertical(false))
            {
                Serial.println("Screen mirror flip vertical flag removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing screen mirror flip vertical flag");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_SET_DEFAULT_SCREEN:
        if (value && strlen(value))
        {
            Serial.printf("Serial command received: set default screen (%s)\n", value);
            if (settings->setDefaultScreen((enum ScreenType)(uint8_t)strtoul(value, nullptr, 10)))
            {
                Serial.println("Default screen saved. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error saving default screen");
            }
        }
        else
        {
            Serial.println("Serial command received: unset default screen");
            if (settings->setDefaultScreen(ST_NONE))
            {
                Serial.println("Default screen removed. Reboot REQUIRED");
            }
            else
            {
                Serial.println("Error removing default screen");
            }
        }
        break;
    case CUSTOM_SERIAL_COMMAND_INDEX_UNKNOWN:
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
    SerialManager::Init(SerialManager::DEFAULT_SPEED);
    Serial.println("Starting esp32-server-dashboard");

    settings = new CustomSettings();

    screenMirrorFlipVertical = settings->getScreenMirrorFlipVertical();

    char WiFiSSID[WiFiManager::MAX_SSID_LENGTH + 1];
    settings->getWIFISSID(WiFiSSID, sizeof(WiFiSSID));

    char WiFiPassword[WiFiManager::MAX_PASSWORD_LENGTH + 1];
    settings->getWIFIPassword(WiFiPassword, sizeof(WiFiPassword));

    WiFiManager::onConnectionStatusChanged(onWifiConnectionStatusChanged);
    WiFiManager::setCredentials(WiFiSSID, WiFiPassword);
    WiFiManager::connect(true);

    sourceData = new SourceData(true, settings->getMaxDownloadBandwidthBytes(), settings->getMaxUploadBandwidthBytes());
    char hostname[SourceData::MAX_HOSTNAME_LENGTH + 1];
    settings->getHostname(hostname, sizeof(hostname));
    sourceData->setHostname(hostname);

#ifdef SOURCE_DUMMY
    dummySRC = new DummySource(sourceData);
#endif // SOURCE_DUMMY
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    screen = new LGFX(PIN_SDA, PIN_SCL, PIN_CS, PIN_DC, PIN_RST, DISPLAY_DRIVER_LOVYANN_ST7789_WIDTH, DISPLAY_DRIVER_LOVYANN_ST7789_HEIGHT, !screenMirrorFlipVertical ? DISPLAY_DRIVER_LOVYANN_ST7789_ROTATION : DISPLAY_DRIVER_LOVYANN_ST7789_ROTATION_MIRROR_FLIP_VERTICAL);
    screen->setSourceData(sourceData);
    screen->initScreen(settings->getDefaultScreen(ST_INFO));
#endif // DISPLAY_DRIVER_LOVYANN_ST7789
    button = new Bounce2::Button();
    button->attach(PIN_BUTTON_SW, INPUT_PULLUP);
    button->interval(5);
    button->setPressedState(LOW);
}

void loop()
{
    // SerialManager::loop();
    SerialManager::Loop(CUSTOM_SERIAL_COMMANDS, CUSTOM_SERIAL_COMMAND_COUNT, onReceivedSerialCommand);
    WiFiManager::loop();
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
    FPS::Loop(999);
#endif // DISPLAY_DRIVER_LOVYANN_ST7789
    yield();
}
