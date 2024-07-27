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

#include "src/utils/WifiManager.hpp"
#include "src/utils/SerialManager.hpp"
#include "src/utils/MQTT.hpp"
#include "src/sources/MeterEntity.hpp"
#include "src/sources/Source.hpp"
#include "src/sources/dummy/DummySource.hpp"
#include "src/sources/mqtt/MQTTTelegrafSource.hpp"
#include "src/Settings/Settings.hpp"

Source *dummySRC = nullptr;
MQTTTelegrafSource *mqttTelegrafSRC = nullptr;
Settings *settings = nullptr;
WifiManager wifi;

enum appScreen
{
    APP_SCREEN_INFO,
    APP_SCREEN_RESUME_DATA
};

appScreen currentScreen = APP_SCREEN_RESUME_DATA;

#define METER_GRAPH_WIDTH 195
#define METER_GRAPH_HEIGHT 30

void setup()
{

    settings = new Settings();
    SerialManager::init(SerialManager::DEFAULT_SPEED);
    Serial.println("Starting esp32-server-dashboard");
    // TODO: default info screen if no valid settings found
    // TODO: rotary encoder controller, button pressed at boot = enter settings mode, movement = toggle between screens
    WifiManager::setCredentials(WIFI_SSID, WIFI_PASSWORD);
    WifiManager::connect(true);
    char mac[32] = {'\0'};
    WifiManager::getMacAddress(mac, sizeof(mac));
    mqttTelegrafSRC = new MQTTTelegrafSource("mqtt://192.168.24.5", mac, "telegraf/HOST_NAME/#");
    // mqttTelegrafSRC->setCPUTopic("telegraf/HOST_NAME/cpu");
    // mqttTelegrafSRC->setMemoryTopic("telegraf/HOST_NAME/mem");
#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    dummySRC = new DummySource();

    screen = new LGFX(PIN_SDA, PIN_SCL, PIN_CS, PIN_DC, PIN_RST, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ROTATION);
    screen->init();
    screen->fillScreen(TFT_BLACK);
    // screen->drawRect(0, 0, 320, 240, TFT_WHITE); // this is for screen bounds debugging purposes only

    if (currentScreen == APP_SCREEN_RESUME_DATA)
    {
        cpuLoadMeter = new LGFXMeter(screen, METER_ENTITY_CPU_LOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 0, TFT_BLACK, "CPU LOAD");
        cpuLoadMeter->setMin(0);
        cpuLoadMeter->setMax(100);
        memoryLoadMeter = new LGFXMeter(screen, METER_ENTITY_MEMORY, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 42, TFT_BLACK, "MEMORY");
        memoryLoadMeter->setMin(0);
        memoryLoadMeter->setMax(34231410688); // 32 Gb
        cpuTemperatureLoadMeter = new LGFXMeter(screen, METER_ENTITY_CPU_TEMPERATURE, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 84, TFT_BLACK, "CPU TEMP");
        cpuTemperatureLoadMeter->setMin(0);
        cpuTemperatureLoadMeter->setMax(100);
        networkDownloadBandwithLoadMeter = new LGFXMeter(screen, METER_ENTITY_NETWORK_BANDWITH_DOWNLOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 126, TFT_BLACK, "WAN DOWNLOAD");
        networkDownloadBandwithLoadMeter->setMin(0);
        networkDownloadBandwithLoadMeter->setMax(536870912); // 512 Mb
        networkUploadBandwithLoadMeter = new LGFXMeter(screen, METER_ENTITY_NETWORK_BANDWITH_UPLOAD, METER_GRAPH_WIDTH, METER_GRAPH_HEIGHT, 0, 168, TFT_BLACK, "WAN UPLOAD");
    }
    else if (currentScreen == APP_SCREEN_INFO)
    {
        screen->initScreenInfo();
    }
#endif
}

bool refreshAlways = false;
uint64_t currentMillis = millis();
void loop()
{
    SerialManager::loop();
    WifiManager::loop();

#ifdef DISPLAY_DRIVER_LOVYANN_ST7789
    if (currentScreen == APP_SCREEN_RESUME_DATA)
    {
        Serial.println("REFRESH");
        // dummySRC->refresh();
        if (refreshAlways || mqttTelegrafSRC->changed(CPU_LOAD, currentMillis))
        {
            cpuLoadMeter->refresh(mqttTelegrafSRC->getCurrentCPULoad());
            Serial.println("REFRESH CPU");
        }
        if (refreshAlways || mqttTelegrafSRC->changed(MEMORY, currentMillis))
        {
            memoryLoadMeter->refresh(mqttTelegrafSRC->getUsedMemory());
        }
        if (refreshAlways || mqttTelegrafSRC->changed(CPU_TEMPERATURE, currentMillis))
        {
            cpuTemperatureLoadMeter->refresh(mqttTelegrafSRC->getCurrentCPUTemperature());
        }
        if (refreshAlways || mqttTelegrafSRC->changed(NETWORK_BANDWITH_DOWNLOAD, currentMillis))
        {
            networkDownloadBandwithLoadMeter->refresh(mqttTelegrafSRC->getUsedNetworkDownloadBandwith());
        }
        if (refreshAlways || mqttTelegrafSRC->changed(NETWORK_BANDWITH_UPLOAD, currentMillis))
        {
            networkUploadBandwithLoadMeter->refresh(mqttTelegrafSRC->getUsedNetworkUploadBandwith());
        }
        screen->refreshDebug(0, 210, WifiManager::getSignalStrength());
        currentMillis = mqttTelegrafSRC->getCurrent()();
    }
    else if (currentScreen == APP_SCREEN_INFO)
    {
        screen->refreshScreenInfo();
    }
#endif
}
