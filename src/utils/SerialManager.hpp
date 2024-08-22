// WARNING: ESP32 C3 SUPER MINI requires "USB CDC on boot" ENABLED (under Arduino IDE Menu -> Tools)

#ifndef APORTELA_MICROCONTROLLER_UTILS_SERIALMANAGER_H
#define APORTELA_MICROCONTROLLER_UTILS_SERIALMANAGER_H

#include <cstdint>
#include <cstdio>

namespace aportela::microcontroller::utils
{
    enum SerialManagerCommand
    {
        SerialManagerCommand_NONE = 0,
        SerialManagerCommand_UNKNOWN = 1,
        SerialManagerCommand_REBOOT = 2,
        SerialManagerCommand_CLEAR_SETTINGS = 3,
        SerialManagerCommand_EXPORT_SETTINGS = 4,
        SerialManagerCommand_CONNECT_WIFI = 5,
        SerialManagerCommand_DISCONNECT_WIFI = 6,
        SerialManagerCommand_SET_WIFI_SSID = 7,
        SerialManagerCommand_SET_WIFI_PASSWORD = 8,
        SerialManagerCommand_SET_MQTT_TELEGRAF_URI = 9,
        SerialManagerCommand_SET_MQTT_TELEGRAF_TOPIC = 10,
        SerialManagerCommand_TOGGLE_SCREEN = 11,
        SerialManagerCommand_SET_MAX_DOWNLOAD_BYTES_BANDWIDTH = 12,
        SerialManagerCommand_SET_MAX_UPLOAD_BYTES_BANDWIDTH = 13,
        SerialManagerCommand_SET_NETWORK_INTERFACE_ID = 14,
        SerialManagerCommand_SET_HOSTNAME = 15,
        SerialManagerCommand_SET_DEFAULT_SCREEN = 16,
    };

    extern const char *SerialCommandStr[];

    typedef void (*SerialCommandCallback)(SerialManagerCommand cmd, const char *value);

    class SerialManager
    {
    private:
        static SerialCommandCallback commandReceivedCallback;

    public:
        static const uint32_t DEFAULT_SPEED = 9600;
        static void init(uint32_t speed, SerialCommandCallback callback);
        static void onCommandReceived(SerialCommandCallback callback);
        static void loop(void);
    };
}

#endif // APORTELA_MICROCONTROLLER_UTILS_SERIALMANAGER_H
