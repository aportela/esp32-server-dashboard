// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/protocols/mqtt.html
// #include <mqtt_client.h>

// #include <Preferences.h>

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

Source *dummySRC = nullptr;

// Preferences preferences;

#define METER_GRAPH_WIDTH 195
#define METER_GRAPH_HEIGHT 30

void setup()
{
    // preferences.begin("esp32-server-dashboard", false);
    // preferences.getString("WIFI_SSID", "");
    // preferences.getString("WIFI_PASSWORD", "");
    const char *ssid = WIFI_SSID;
    const char *password = WIFI_PASSWORD;
    WiFi.begin(ssid, password);
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    dummySRC = new DummySource();
    screen = new LGFX(PIN_SDA, PIN_SCL, PIN_CS, PIN_DC, PIN_RST, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ROTATION);
    screen->init();
    screen->fillScreen(TFT_BLACK);
    // screen->drawRect(0, 0, 320, 240, TFT_WHITE); // this is for screen bounds debugging purposes only
    screen->setSource(dummySRC);
    cpuLoadMeter = new LGFXMeter(screen, METER_ENTITY_CPU_LOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 0, TFT_BLACK, "CPU LOAD");
    memoryLoadMeter = new LGFXMeter(screen, METER_ENTITY_MEMORY, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 42, TFT_BLACK, "MEMORY");
    cpuTemperatureLoadMeter = new LGFXMeter(screen, METER_ENTITY_CPU_TEMPERATURE, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 84, TFT_BLACK, "CPU TEMP");
    networkDownloadBandwithLoadMeter = new LGFXMeter(screen, METER_ENTITY_NETWORK_BANDWITH_DOWNLOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 126, TFT_BLACK, "WAN DOWNLOAD");
    networkUploadBandwithLoadMeter = new LGFXMeter(screen, METER_ENTITY_NETWORK_BANDWITH_UPLOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 168, TFT_BLACK, "WAN UPLOAD");

#endif
}

bool WIFIFirstConnectionSuccess = false;
bool WIFIForcingReconnection = false;

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        // waiting for first connection
        if (!WIFIFirstConnectionSuccess)
        {
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
            }
            else
            {
                // waiting for reconnection
                delay(500);
            }
        }
    }
    else
    {
        // this controls that connection is valid
        if (!WIFIFirstConnectionSuccess)
        {
            WIFIFirstConnectionSuccess = true;
        }
        WIFIForcingReconnection = false;
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
        dummySRC->refresh();
        cpuLoadMeter->refresh(dummySRC->getCurrentCPULoad());
        memoryLoadMeter->refresh(dummySRC->getUsedMemory());
        cpuTemperatureLoadMeter->refresh(dummySRC->getCurrentCPUTemperature());
        networkDownloadBandwithLoadMeter->refresh(dummySRC->getUsedNetworkDownloadBandwith());
        networkUploadBandwithLoadMeter->refresh(dummySRC->getUsedNetworkUploadBandwith());
#endif
    }
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    screen->refreshDebug(0, 210, WiFi.RSSI());
#endif
}
