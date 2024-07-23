#ifndef ESP32_SERVER_DASHBOARD_FORMAT_UTILS_H
#define ESP32_SERVER_DASHBOARD_FORMAT_UTILS_H

#include <cstdint>
#include <cstdio>

class Format
{
private:
public:
    Format(void);
    ~Format();

    static void millisToString(uint64_t millis_diff, char *buffer, size_t buffer_size);
};

#endif
