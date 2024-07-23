// WARNING: ESP32 C3 SUPER MINI requires "USB CDC on boot" ENABLED (under Arduino IDE Menu -> Tools)

#ifndef ESP32_SERVER_DASHBOARD_SERIAL_MANAGER_H
#define ESP32_SERVER_DASHBOARD_SERIAL_MANAGER_H

#include <cstdint>
#include <cstdio>

class SerialManager
{
public:
    static const uint32_t DEFAULT_SPEED = 9600;
    static void init(uint32_t speed);
    static void loop(void);
};

#endif
