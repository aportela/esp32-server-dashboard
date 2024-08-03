#include "Format.hpp"
#include <string.h>
#include <Arduino.h>
#include <inttypes.h>

#define BYTE_UNIT_DIVISOR 1024

void Format::parseFloatIntoCharArray(float value, uint8_t decimalCount, uint8_t leftZeroPaddingCount, char *buffer, size_t buffer_size)
{
    char strDecimalValue[32] = {'\0'};
    std::snprintf(strDecimalValue, sizeof(strDecimalValue), "%.2f", value); // set 2 decimals
    char format[8] = {'\0'};
    std::snprintf(format, sizeof(format), "%%0%us", leftZeroPaddingCount); // set dynamic format left zero padding
    std::snprintf(buffer, buffer_size, format, strDecimalValue);           // set left zero padding
}

void Format::bytesToHumanStr(uint64_t bytes, char *buffer, size_t buffer_size, bool useBandWithUnits)
{
    if (bytes > 0)
    {
        static const char *units[] = {"Bytes", "KBytes", "MBytes", "GBytes", "TBytes", "PBytes", "EBytes"};
        static const char *bandWithUnits[] = {"Bytes/seg", "KBytes/seg", "MBytes/seg", "GBytes/seg", "TBytes/seg", "PBytes/seg", "EBytes/seg"};
        static const uint8_t numUnits = sizeof(units) / sizeof(units[0]) - 1;
        uint8_t currentUnitIndex = 0;
        uint64_t tmpBytes = bytes - (bytes > BYTE_UNIT_DIVISOR ? bytes % BYTE_UNIT_DIVISOR : 0);
        while (tmpBytes >= BYTE_UNIT_DIVISOR && currentUnitIndex < numUnits)
        {
            tmpBytes /= BYTE_UNIT_DIVISOR;
            currentUnitIndex++;
        }
        std::snprintf(buffer, buffer_size, "%04" PRIu64 " %s", tmpBytes, useBandWithUnits ? bandWithUnits[currentUnitIndex] : units[currentUnitIndex]);
    }
    else
    {
        std::snprintf(buffer, buffer_size, useBandWithUnits ? "0000 Bytes/seg" : "0000 Bytes");
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
