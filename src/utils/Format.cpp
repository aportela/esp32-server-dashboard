#include "Format.hpp"
#include <string.h>
#include <Arduino.h>
#include <inttypes.h>

#define BYTE_UNIT_DIVISOR 1024

void Format::bytesToHumanStr(uint64_t bytes, char *buffer, size_t buffer_size)
{
    static const char *units[] = {"B  ", "Kb ", "Mb ", "Gb ", "Tb ", "Pb "};
    static const uint8_t numUnits = sizeof(units) / sizeof(units[0]) - 1;
    uint8_t currentUnitIndex = 0;
    double decValue = (double)bytes;
    while (decValue >= 1024.0 && currentUnitIndex < numUnits)
    {
        decValue /= 1024;
        currentUnitIndex++;
    }
    std::sprintf(buffer, "%03d", (int)decValue);
    strcat(buffer, units[currentUnitIndex]);
}

/// @brief 4 digits + 1 decimal
/// @param bytes
/// @param buffer
/// @param buffer_size
void Format::_bytesToHumanStr(uint64_t bytes, char *buffer, size_t buffer_size)
{
    if (bytes > 0)
    {
        static const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
        static const uint8_t numUnits = sizeof(units) / sizeof(units[0]) - 1;
        uint8_t currentUnitIndex = 0;

        uint16_t mod = bytes > BYTE_UNIT_DIVISOR ? bytes % BYTE_UNIT_DIVISOR : 0;
        uint64_t tmpBytes = bytes - mod;

        while (tmpBytes >= BYTE_UNIT_DIVISOR && currentUnitIndex < numUnits)
        {
            tmpBytes /= BYTE_UNIT_DIVISOR;
            currentUnitIndex++;
        }
        // TODO: better round to nearest decimal
        while (mod > 10)
        {
            mod /= 10;
        }
        std::sprintf(buffer, "%04" PRIu64 ".%01u", tmpBytes, mod);
        strcat(buffer, units[currentUnitIndex]);
    }
    else
    {
        snprintf(buffer, buffer_size, "0000.0B");
    }
}

void Format::millisToHumanStr(uint64_t millis_diff, char *buffer, size_t buffer_size)
{
    static const uint64_t millis_in_second = 1000;
    static const uint64_t millis_in_minute = millis_in_second * 60;
    static const uint64_t millis_in_hour = millis_in_minute * 60;
    static const uint64_t millis_in_day = millis_in_hour * 24;
    static const uint64_t millis_in_month = millis_in_day * 30;
    static const uint64_t millis_in_year = millis_in_day * 365;

    double time = 0.0;
    const char *unit = "";

    if (millis_diff >= millis_in_year)
    {
        time = (double)millis_diff / millis_in_year;
        unit = (time >= 2) ? "years" : "year";
    }
    else if (millis_diff >= millis_in_month)
    {
        time = (double)millis_diff / millis_in_month;
        unit = (time >= 2) ? "months" : "month";
    }
    else if (millis_diff >= millis_in_day)
    {
        time = (double)millis_diff / millis_in_day;
        unit = (time >= 2) ? "days" : "day";
    }
    else if (millis_diff >= millis_in_hour)
    {
        time = (double)millis_diff / millis_in_hour;
        unit = (time >= 2) ? "hours" : "hour";
    }
    else if (millis_diff >= millis_in_minute)
    {
        time = (double)millis_diff / millis_in_minute;
        unit = (time >= 2) ? "mins" : "min";
    }
    else
    {
        time = (double)millis_diff / millis_in_second;
        unit = (time >= 2) ? "secs" : "sec";
    }

    std::snprintf(buffer, buffer_size, "%.1f %s", time, unit);
}
