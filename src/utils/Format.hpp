#ifndef APORTELA_MICROCONTROLLER_UTILS_FORMAT_H
#define APORTELA_MICROCONTROLLER_UTILS_FORMAT_H

#include <cstdint>
#include <cstdio>

namespace aportela::microcontroller::utils
{
    extern const char *const ShortByteUnits[];
    extern const char *const LongByteUnits[];
    extern const char *const ShortBandwidthByteUnits[];
    extern const char *const LongBandwidthByteUnits[];

    enum KILO_BYTE_DIVISOR
    {
        KBD_1024 = 1024,
        KBD_1000 = 1000
    };

    class Format
    {
    private:
    public:
        static void parseFloatIntoCharArray(float value, uint8_t decimalCount, uint8_t leftZeroPaddingCount, char *buffer, size_t bufferSize);
        static void bytesToHumanStr(uint64_t bytes, char *buffer, size_t bufferSize, bool zeroPadding = false, bool shortUnits = false, bool bandwidthUnits = false, KILO_BYTE_DIVISOR kByteDivisorUnit = KBD_1024);
        static void millisToHumanStr(uint64_t millis_diff, char *buffer, size_t bufferSize);
    };
}

#endif // APORTELA_MICROCONTROLLER_UTILS_FORMAT_H
