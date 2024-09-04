#ifndef APORTELA_MICROCONTROLLER_UTILS_FORMAT_H
#define APORTELA_MICROCONTROLLER_UTILS_FORMAT_H

#include <cstdint>
#include <cstdio>

namespace aportela::microcontroller::utils
{
    extern const char *const SHORT_BYTE_UNITS[];
    extern const char *const LONG_BYTE_UNITS[];
    extern const char *const SHORT_BANDWITH_BYTE_UNITS[];
    extern const char *const LONG_BANDWITH_BYTE_UNITS[];

    enum KILO_BYTE_DIVISOR
    {
        KILO_BYTE_DIVISOR_1024 = 1024,
        KILO_BYTE_DIVISOR_1000 = 1000
    };

    class Format
    {
    private:
    public:
        static void ParseFloatToString(float value, uint8_t decimalCount, uint8_t leftZeroPaddingCount, char *buffer, size_t bufferSize);
        static void ParseBytesToHumanString(uint64_t bytes, char *buffer, size_t bufferSize, bool zeroPadding = false, bool shortUnits = false, bool bandwidthUnits = false, KILO_BYTE_DIVISOR kByteDivisorUnit = KILO_BYTE_DIVISOR_1024);
        static void ParseMillisToHumanString(uint64_t millisDiff, char *buffer, size_t bufferSize);
    };
}

#endif // APORTELA_MICROCONTROLLER_UTILS_FORMAT_H
