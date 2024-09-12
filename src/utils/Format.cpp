#include "Format.hpp"
#include <cinttypes>

namespace aportela::microcontroller::utils
{
#define BYTE_UNITS_SIZE 7 // next const arrays size

    const char *const SHORT_BYTE_UNITS[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
    const char *const LONG_BYTE_UNITS[] = {"Bytes", "KBytes", "MBytes", "GBytes", "TBytes", "PBytes", "EBytes"};
    const char *const SHORT_BANDWITH_BYTE_UNITS[] = {"B/s", "KB/s", "MB/s", "GB/s", "TB/s", "PB/s", "EB/s"};
    const char *const LONG_BANDWITH_BYTE_UNITS[] = {"Bytes/seg", "KBytes/seg", "MBytes/seg", "GBytes/seg", "TBytes/seg", "PBytes/seg", "EBytes/seg"};

#define BITS_UNITS_SIZE 7 // next const arrays size
    const char *const SHORT_BIT_UNITS[] = {"b", "Kb", "Mb", "Gb", "Tb", "Pb", "Eb"};
    const char *const LONG_BIT_UNITS[] = {"bits", "Kbits", "Mbits", "Gbits", "Tbit", "Pbits", "Ebits"};
    const char *const SHORT_BANDWITH_BIT_UNITS[] = {"b/s", "Kb/s", "Mb/s", "Gb/s", "Tb/s", "Pb/s", "Eb/s"};
    const char *const LONG_BANDWITH_BIT_UNITS[] = {"bits/seg", "Kbits/seg", "Mbits/seg", "Gbits/seg", "Tbits/seg", "Pbits/seg", "Ebits/seg"};

    void Format::ParseFloatToString(float value, char *buffer, size_t bufferSize, uint8_t decimalCount, uint8_t leftZeroPaddingToCharCount)
    {
        char decimalFormatPattern[8] = {'\0'};
        std::snprintf(decimalFormatPattern, sizeof(decimalFormatPattern), "%%.%df", decimalCount); // => %.2f (for 2 decimals)
        char strDecimalValue[32] = {'\0'};
        std::snprintf(strDecimalValue, sizeof(strDecimalValue), decimalFormatPattern, value); // => 24.96 (for previous 2 decimal format on float value = 24.96102)
        if (leftZeroPaddingToCharCount > 0)
        {
            char finalFormatWithZeroPadding[8] = {'\0'};
            std::snprintf(finalFormatWithZeroPadding, sizeof(finalFormatWithZeroPadding), "%%0%us", leftZeroPaddingToCharCount); // => %06s (for 6 total chars with left zero padding)
            std::snprintf(buffer, bufferSize, finalFormatWithZeroPadding, strDecimalValue);                                      // => 024.96 (for previous "24.96" plus left zero padding to 6 chars)
        }
        else
        {
            std::snprintf(buffer, bufferSize, "%s", strDecimalValue);
        }
    }

    void Format::ParseBitsToHumanString(uint64_t bits, char *buffer, size_t bufferSize, bool zeroPadding, bool shortUnits, bool bandwidthUnits, KILO_BIT_DIVISOR kBitDivisorUnit)
    {
        if (bits > 0)
        {
            uint8_t currentUnitIndex = 0;
            uint64_t tmpBits = bits;
            while (tmpBits >= kBitDivisorUnit && currentUnitIndex < BITS_UNITS_SIZE - 1)
            {
                tmpBits /= kBitDivisorUnit;
                currentUnitIndex++;
            }
            const char *bitUnitStr = (shortUnits ? (bandwidthUnits ? SHORT_BANDWITH_BIT_UNITS : SHORT_BIT_UNITS) : (bandwidthUnits ? LONG_BANDWITH_BIT_UNITS : LONG_BIT_UNITS))[currentUnitIndex];
            std::snprintf(buffer, bufferSize, zeroPadding ? "%04" PRIu64 "%s" : "%" PRIu64 "%s", tmpBits, bitUnitStr);
        }
        else
        {
            const char *minBitUnitStr = (shortUnits ? (bandwidthUnits ? SHORT_BANDWITH_BIT_UNITS : SHORT_BIT_UNITS) : (bandwidthUnits ? LONG_BANDWITH_BIT_UNITS : LONG_BIT_UNITS))[0];
            std::snprintf(buffer, bufferSize, zeroPadding ? "0000%s" : "0%s", minBitUnitStr);
        }
    }

    void Format::ParseBytesToHumanString(uint64_t bytes, char *buffer, size_t bufferSize, bool zeroPadding, bool shortUnits, bool bandwidthUnits, KILO_BYTE_DIVISOR kByteDivisorUnit)
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
            const char *byteUnitStr = (shortUnits ? (bandwidthUnits ? SHORT_BANDWITH_BYTE_UNITS : SHORT_BYTE_UNITS) : (bandwidthUnits ? LONG_BANDWITH_BYTE_UNITS : LONG_BYTE_UNITS))[currentUnitIndex];
            std::snprintf(buffer, bufferSize, zeroPadding ? "%04" PRIu64 "%s" : "%" PRIu64 "%s", tmpBytes, byteUnitStr);
        }
        else
        {
            const char *minByteUnitStr = (shortUnits ? (bandwidthUnits ? SHORT_BANDWITH_BYTE_UNITS : SHORT_BYTE_UNITS) : (bandwidthUnits ? LONG_BANDWITH_BYTE_UNITS : LONG_BYTE_UNITS))[0];
            std::snprintf(buffer, bufferSize, zeroPadding ? "0000%s" : "0%s", minByteUnitStr);
        }
    }

    void Format::ParseMillisToHumanString(uint64_t millisDiff, char *buffer, size_t bufferSize)
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
