#include "FormatUtils.hpp"

void FormatUtils::millisToString(uint64_t millis_diff, char *buffer, size_t buffer_size)
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
        unit = (time > 1) ? "years" : "year";
    }
    else if (millis_diff >= millis_in_month)
    {
        time = (double)millis_diff / millis_in_month;
        unit = (time > 1) ? "months" : "month";
    }
    else if (millis_diff >= millis_in_day)
    {
        time = (double)millis_diff / millis_in_day;
        unit = (time > 1) ? "days" : "day";
    }
    else if (millis_diff >= millis_in_hour)
    {
        time = (double)millis_diff / millis_in_hour;
        unit = (time > 1) ? "hours" : "hour";
    }
    else if (millis_diff >= millis_in_minute)
    {
        time = (double)millis_diff / millis_in_minute;
        unit = (time > 1) ? "mins" : "min";
    }
    else
    {
        time = (double)millis_diff / millis_in_second;
        unit = (time > 1) ? "secs" : "sec";
    }

    std::snprintf(buffer, buffer_size, "%.1f %s", time, unit);
}
