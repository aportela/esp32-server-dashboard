// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/protocols/mqtt.html
// #include <mqtt_client.h>

// #include <Preferences.h>

// WARNING: ESP32 C3 SUPER MINI requires "USB CDC on boot" ENABLED (under Arduino IDE Menu -> Tools)
#define SERIAL_SPEED 9600

#define DISPLAY_DRIVER_LOVYANN_ST7789 // at this time, only LovyAnn ST7789 driver support

#ifdef DISPLAY_DRIVER_LOVYANN_ST7789

#include "src/display/ST7789-SPI-240x320-Lovyann/LGFX.hpp"
#include "src/display/ST7789-SPI-240x320-Lovyann/LGFXMeter.hpp"

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

// these are my custom/valid values for a ST7789 240x320 screen
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define SCREEN_ROTATION 3

LGFX *screen = nullptr;
LGFXMeter *cpuLoadMeter = nullptr;
LGFXMeter *memoryLoadMeter = nullptr;
LGFXMeter *cpuTemperatureLoadMeter = nullptr;
LGFXMeter *networkDownloadBandwithLoadMeter = nullptr;
LGFXMeter *networkUploadBandwithLoadMeter = nullptr;

#else
#error NO_DISPLAY_DRIVER
#endif

#include "WiFi.h"
#include "WiFi-Settings.h"
#include "src/sources/MeterEntity.hpp"
#include "src/sources/Source.hpp"
#include "src/sources/dummy/DummySource.hpp"
#include "src/Settings/Settings.hpp"

Source *dummySRC = nullptr;

Settings *settings = nullptr;

#define METER_GRAPH_WIDTH 195
#define METER_GRAPH_HEIGHT 30

void initSerialPort()
{
    Serial.begin(SERIAL_SPEED);
    while (!Serial)
    {
        yield();
        delay(10);
    }
}

void processSerialPort()
{
    while (Serial.available() > 0)
    {
        String rx = Serial.readStringUntil('\n');
        if (rx == "REBOOT")
        {
            Serial.println("Rebooting");
            ESP.restart();
        }
        if (rx == "CLEAR_SETTINGS")
        {
            Serial.println("Reseting settings");
            settings->clear();
        }
        else if (rx.startsWith("SET_WIFI_SSID ") && rx.length() > 14)
        {
            String rxSSID = rx.substring(14);
            if (rxSSID.length() > 0)
            {
                Serial.println("Received new SSID");
                Serial.println(rxSSID);
                settings->setWIFISSID(rxSSID.c_str());
            }
        }
        else if (rx.startsWith("SET_WIFI_PASSWORD "))
        {
            String rxPassword = rx.substring(18);
            if (rxPassword.length() > 0)
            {
                Serial.println("Received new password");
                Serial.println(rxPassword);
                settings->setWIFIPassword(rxPassword.c_str());
            }
        }
        else
        {
            Serial.printf("Unknown cmd %s\n", rx.c_str());
        }
    }
}

bool WIFIEnabled = false;
void setup()
{
    settings = new Settings();
    initSerialPort();
    Serial.println("Starting esp32-server-dashboard");
    // TODO: default info screen if no valid settings found
    // TODO: rotary encoder controller, button pressed at boot = enter settings mode, movement = toggle between screens
    char *ssid = WIFI_SSID;
    char *password = WIFI_PASSWORD;
    // char ssid[33] = {'\0'};
    // char password[65] = {'\0'};
    // settings->getWIFISSID(ssid, 32);
    // settings->getWIFIPassword(password, 64);
    WIFIEnabled = strlen(ssid) > 0;
    if (WIFIEnabled)
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
    }
    else
    {
        Serial.println("Previous WIFI SSID not found, send via serial port 'SET_WIFI_SSSID xxxxxxxxxxxxx'");
    }
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    dummySRC = new DummySource();
    screen = new LGFX(PIN_SDA, PIN_SCL, PIN_CS, PIN_DC, PIN_RST, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ROTATION);
    screen->init();
    screen->fillScreen(TFT_BLACK);
    screen->drawRect(0, 0, 320, 240, TFT_WHITE); // this is for screen bounds debugging purposes only
    /*
    screen->setSource(dummySRC);
    cpuLoadMeter = new LGFXMeter(screen, METER_ENTITY_CPU_LOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 0, TFT_BLACK, "CPU LOAD");
    memoryLoadMeter = new LGFXMeter(screen, METER_ENTITY_MEMORY, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 42, TFT_BLACK, "MEMORY");
    cpuTemperatureLoadMeter = new LGFXMeter(screen, METER_ENTITY_CPU_TEMPERATURE, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 84, TFT_BLACK, "CPU TEMP");
    networkDownloadBandwithLoadMeter = new LGFXMeter(screen, METER_ENTITY_NETWORK_BANDWITH_DOWNLOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 126, TFT_BLACK, "WAN DOWNLOAD");
    networkUploadBandwithLoadMeter = new LGFXMeter(screen, METER_ENTITY_NETWORK_BANDWITH_UPLOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 168, TFT_BLACK, "WAN UPLOAD");
    */
    screen->initScreenInfo();
#endif
}

bool WIFIFirstConnectionSuccess = false;
bool WIFIForcingReconnection = false;

void loop()
{
    processSerialPort();
    if (WiFi.status() != WL_CONNECTED)
    {
        if (WIFIEnabled)
        {
            // waiting for first connection
            if (!WIFIFirstConnectionSuccess)
            {
                Serial.println("Waiting for WIFI connection");
                delay(500);
            }
            else
            {
                // previous conection lost
                if (!WIFIForcingReconnection)
                {
                    // force new reconnection
                    WiFi.reconnect();
                    WIFIForcingReconnection = true;
                    Serial.println("Forcing WIFI reconnection");
                }
                else
                {
                    Serial.println("Waiting for WIFI reconnection");
                    // waiting for reconnection
                    delay(500);
                }
            }
        }
    }
    else
    {
        if (WIFIEnabled)
        {
            // this controls that connection is valid
            if (!WIFIFirstConnectionSuccess)
            {
                WIFIFirstConnectionSuccess = true;
            }
            WIFIForcingReconnection = false;
        }
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
        /*
            dummySRC->refresh();
            cpuLoadMeter->refresh(dummySRC->getCurrentCPULoad());
            memoryLoadMeter->refresh(dummySRC->getUsedMemory());
            cpuTemperatureLoadMeter->refresh(dummySRC->getCurrentCPUTemperature());
            networkDownloadBandwithLoadMeter->refresh(dummySRC->getUsedNetworkDownloadBandwith());
            networkUploadBandwithLoadMeter->refresh(dummySRC->getUsedNetworkUploadBandwith());
        */
#endif
    }
    screen->refreshScreenInfo();
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    // screen->refreshDebug(0, 210, WiFi.RSSI());
#endif
}
