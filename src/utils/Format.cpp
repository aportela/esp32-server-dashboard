#include "Format.hpp"
#include <string.h>
#include <Arduino.h>
#include <inttypes.h>

#define BYTE_UNIT_DIVISOR 1024
#define BYTE_UNITS_SIZE 7 // next const arrays size

const char *const ShortByteUnits[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
const char *const LongByteUnits[] = {"Bytes", "KBytes", "MBytes", "GBytes", "TBytes", "PBytes", "EBytes"};
const char *const ShortBandwidthByteUnits[] = {"B/s", "KB/s", "MB/s", "GB/s", "TB/s", "PB/s", "EB/s"};
const char *const LongBandwidthByteUnits[] = {"Bytes/seg", "KBytes/seg", "MBytes/seg", "GBytes/seg", "TBytes/seg", "PBytes/seg", "EBytes/seg"};

void Format::parseFloatIntoCharArray(float value, uint8_t decimalCount, uint8_t leftZeroPaddingCount, char *buffer, size_t buffer_size)
{
    char strDecimalValue[32] = {'\0'};
    std::snprintf(strDecimalValue, sizeof(strDecimalValue), "%.2f", value); // set 2 decimals
    char format[8] = {'\0'};
    std::snprintf(format, sizeof(format), "%%0%us", leftZeroPaddingCount); // set dynamic format left zero padding
    std::snprintf(buffer, buffer_size, format, strDecimalValue);           // set left zero padding
}

void Format::bytesToHumanStr(uint64_t bytes, char *buffer, size_t buffer_size, bool zeroPadding, bool shortUnits, bool bandwidthUnits)
{
    if (bytes > 0)
    {
        uint8_t currentUnitIndex = 0;
        uint64_t tmpBytes = bytes - (bytes > BYTE_UNIT_DIVISOR ? bytes % BYTE_UNIT_DIVISOR : 0);
        while (tmpBytes >= BYTE_UNIT_DIVISOR && currentUnitIndex < BYTE_UNITS_SIZE)
        {
            tmpBytes /= BYTE_UNIT_DIVISOR;
            currentUnitIndex++;
        }
        if (shortUnits)
        {
            if (bandwidthUnits)
            {
                std::snprintf(buffer, buffer_size, zeroPadding ? "%04" PRIu64 "%s" : "%" PRIu64 "%s", tmpBytes, ShortBandwidthByteUnits[currentUnitIndex]);
            }
            else
            {
                std::snprintf(buffer, buffer_size, zeroPadding ? "%04" PRIu64 "%s" : "%" PRIu64 "%s", tmpBytes, ShortByteUnits[currentUnitIndex]);
            }
        }
        else
        {
            if (bandwidthUnits)
            {
                std::snprintf(buffer, buffer_size, zeroPadding ? "%04" PRIu64 " %s" : "%" PRIu64 " %s", tmpBytes, LongBandwidthByteUnits[currentUnitIndex]);
            }
            else
            {
                std::snprintf(buffer, buffer_size, zeroPadding ? "%04" PRIu64 " %s" : "%" PRIu64 " %s", tmpBytes, LongByteUnits[currentUnitIndex]);
            }
        }
    }
    else
    {
        if (shortUnits)
        {
            if (bandwidthUnits)
            {
                std::snprintf(buffer, buffer_size, zeroPadding ? "%04" PRIu64 "%s" : "%" PRIu64 "%s", 0, ShortBandwidthByteUnits[0]);
            }
            else
            {
                std::snprintf(buffer, buffer_size, zeroPadding ? "%04" PRIu64 "%s" : "%" PRIu64 "%s", 0, ShortByteUnits[0]);
            }
        }
        else
        {
            if (bandwidthUnits)
            {
                std::snprintf(buffer, buffer_size, zeroPadding ? "%04" PRIu64 " %s" : "%" PRIu64 " %s", 0, LongBandwidthByteUnits[0]);
            }
            else
            {
                std::snprintf(buffer, buffer_size, zeroPadding ? "%04" PRIu64 " %s" : "%" PRIu64 " %s", 0, LongByteUnits[0]);
            }
        }
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
