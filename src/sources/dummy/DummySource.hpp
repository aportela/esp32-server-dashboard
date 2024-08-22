#ifndef ESP32_SERVER_DASHBOARD_DUMMY_SOURCE_H
#define ESP32_SERVER_DASHBOARD_DUMMY_SOURCE_H

#include <stdint.h>
#include "../Source.hpp"

class DummySource : public Source
{
private:
    uint64_t lastEllapsedMillis = 0;

public:
    DummySource(SourceData *sourceData);
    ~DummySource();

    void refresh(uint16_t milliSeconds = 0);
};

#endif // ESP32_SERVER_DASHBOARD_DUMMY_SOURCE_H
