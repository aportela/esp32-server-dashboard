// WARNING: ESP32 C3 SUPER MINI requires "USB CDC on boot" ENABLED (under Arduino IDE Menu -> Tools)

#ifndef ESP32_SERVER_DASHBOARD_SERIAL_MANAGER_H
#define ESP32_SERVER_DASHBOARD_SERIAL_MANAGER_H

#include <cstdint>
#include <cstdio>

enum SerialCommandType
{
    SERIAL_CMDT_NONE = 0,
    SERIAL_CMDT_UNKNOWN = 1,
    SERIAL_CMDT_REBOOT = 2,
    SERIAL_CMDT_CLEAR_SETTINGS = 3,
    SERIAL_CMDT_EXPORT_SETTINGS = 4,
    SERIAL_CMDT_CONNECT_WIFI = 5,
    SERIAL_CMDT_DISCONNECT_WIFI = 6,
    SERIAL_CMDT_SET_WIFI_SSID = 7,
    SERIAL_CMDT_SET_WIFI_PASSWORD = 8,
    SERIAL_CMDT_SET_MQTT_TELEGRAF_URI = 9,
    SERIAL_CMDT_SET_MQTT_TELEGRAF_TOPIC = 10,
    SERIAL_CMDT_TOGGLE_SCREEN = 11,
    SERIAL_CMDT_SET_MIN_CPU_TEMPERATURE = 12,
    SERIAL_CMDT_SET_MAX_CPU_TEMPERATURE = 13,
    SERIAL_CMDT_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH = 14,
    SERIAL_CMDT_SET_MAX_UPLOAD_BYTES_BANDWIDTH = 15,
    SERIAL_CMDT_SET_NETWORK_INTERFACE_ID = 16,
    SERIAL_CMDT_SET_NETWORK_INTERFACE_NAME = 17,
};

extern const char *SerialCommandStr[];

typedef void (*SerialCommandCallback)(SerialCommandType cmd, const char *value);

class SerialManager
{
private:
    static SerialCommandCallback remoteCallback;

public:
    static const uint32_t DEFAULT_SPEED = 9600;
    static void init(uint32_t speed, SerialCommandCallback callback);
    static void setCallback(SerialCommandCallback callback);
    static void loop(void);
};

#endif
