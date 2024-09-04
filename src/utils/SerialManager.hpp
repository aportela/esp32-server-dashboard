// WARNING: ESP32 C3 SUPER MINI requires "USB CDC on boot" ENABLED (under Arduino IDE Menu -> Tools)

#ifndef APORTELA_MICROCONTROLLER_UTILS_SERIALMANAGER_H
#define APORTELA_MICROCONTROLLER_UTILS_SERIALMANAGER_H

#include <cstdint>
#include <cstdio>

namespace aportela::microcontroller::utils
{
    typedef void (*SerialCommandCallback)(int8_t commandIndex, const char *value);

    class SerialManager
    {
    public:
        static const uint32_t DEFAULT_SPEED = 9600;
        static void Init(uint32_t speed);
        static void Loop(const char *commands[], size_t numCommands, SerialCommandCallback callback);
    };
}

#endif // APORTELA_MICROCONTROLLER_UTILS_SERIALMANAGER_H
