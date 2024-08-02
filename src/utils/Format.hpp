#ifndef ESP32_SERVER_DASHBOARD_FORMAT_UTILS_H
#define ESP32_SERVER_DASHBOARD_FORMAT_UTILS_H

#include <cstdint>
#include <cstdio>

class Format
{
private:
public:
    static void parseFloatIntoCharArray(float value, uint8_t decimalCount, uint8_t leftZeroPaddingCount, char *buffer, size_t buffer_size);
    static void bytesToHumanStr(uint64_t bytes, char *buffer, size_t buffer_size);
    static void millisToHumanStr(uint64_t millis_diff, char *buffer, size_t buffer_size);
};

#endif
