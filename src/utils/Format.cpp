#include "Format.hpp"
#include <string.h>
#include <Arduino.h>
#include <inttypes.h>

#define BYTE_UNITS_SIZE 7 // next const arrays size

namespace aportela::microcontroller::utils
{
    const char *const ShortByteUnits[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
    const char *const LongByteUnits[] = {"Bytes", "KBytes", "MBytes", "GBytes", "TBytes", "PBytes", "EBytes"};
    const char *const ShortBandwidthByteUnits[] = {"B/s", "KB/s", "MB/s", "GB/s", "TB/s", "PB/s", "EB/s"};
    const char *const LongBandwidthByteUnits[] = {"Bytes/seg", "KBytes/seg", "MBytes/seg", "GBytes/seg", "TBytes/seg", "PBytes/seg", "EBytes/seg"};

    void Format::parseFloatIntoCharArray(float value, uint8_t decimalCount, uint8_t leftZeroPaddingCount, char *buffer, size_t bufferSize)
    {
        char strDecimalValue[32] = {'\0'};
        std::snprintf(strDecimalValue, sizeof(strDecimalValue), "%.2f", value); // set 2 decimals
        char format[8] = {'\0'};
        std::snprintf(format, sizeof(format), "%%0%us", leftZeroPaddingCount); // set dynamic format left zero padding
        std::snprintf(buffer, bufferSize, format, strDecimalValue);            // set left zero padding
    }

    void Format::bytesToHumanStr(uint64_t bytes, char *buffer, size_t bufferSize, bool zeroPadding, bool shortUnits, bool bandwidthUnits, KILO_BYTE_DIVISOR kByteDivisorUnit)
    {
        if (bytes > 0)
        {
            uint8_t currentUnitIndex = 0;
            uint64_t tmpBytes = bytes;
            while (tmpBytes >= kByteDivisorUnit && currentUnitIndex < BYTE_UNITS_SIZE - 1)
            {
                tmpBytes /= kByteDivisorUnit;
                currentUnitIndex++;
            }
            const char *byteUnitStr = (shortUnits ? (bandwidthUnits ? ShortBandwidthByteUnits : ShortByteUnits) : (bandwidthUnits ? LongBandwidthByteUnits : LongByteUnits))[currentUnitIndex];
            std::snprintf(buffer, bufferSize, zeroPadding ? "%04" PRIu64 "%s" : "%" PRIu64 "%s", tmpBytes, byteUnitStr);
        }
        else
        {
            const char *minByteUnitStr = (shortUnits ? (bandwidthUnits ? ShortBandwidthByteUnits : ShortByteUnits) : (bandwidthUnits ? LongBandwidthByteUnits : LongByteUnits))[0];
            std::snprintf(buffer, bufferSize, zeroPadding ? "0000%s" : "0%s", minByteUnitStr);
        }
    }

    void Format::millisToHumanStr(uint64_t millisDiff, char *buffer, size_t bufferSize)
    {
        static const uint64_t totalMillisInSecond = 1000;
        static const uint64_t totalMillisInMinute = totalMillisInSecond * 60;
        static const uint64_t totalMillisInHour = totalMillisInMinute * 60;
        static const uint64_t totalMillisInDay = totalMillisInHour * 24;
        static const uint64_t totalMillisInMonth = totalMillisInDay * 30;
        static const uint64_t totalMillisInYear = totalMillisInDay * 365;
        double time = 0.0;
        const char *unit = "";
        if (millisDiff >= totalMillisInYear)
        {
            time = (double)millisDiff / totalMillisInYear;
            unit = (time >= 2) ? "years" : "year";
        }
        else if (millisDiff >= totalMillisInMonth)
        {
            time = (double)millisDiff / totalMillisInMonth;
            unit = (time >= 2) ? "months" : "month";
        }
        else if (millisDiff >= totalMillisInDay)
        {
            time = (double)millisDiff / totalMillisInDay;
            unit = (time >= 2) ? "days" : "day";
        }
        else if (millisDiff >= totalMillisInHour)
        {
            time = (double)millisDiff / totalMillisInHour;
            unit = (time >= 2) ? "hours" : "hour";
        }
        else if (millisDiff >= totalMillisInMinute)
        {
            time = (double)millisDiff / totalMillisInMinute;
            unit = (time >= 2) ? "mins" : "min";
        }
        else
        {
            time = (double)millisDiff / totalMillisInSecond;
            unit = (time >= 2) ? "secs" : "sec";
        }
        std::snprintf(buffer, bufferSize, "%.1f %s", time, unit);
    }
}
