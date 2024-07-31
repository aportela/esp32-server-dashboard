#ifndef ESP32_SERVER_DASHBOARD_DUMMY_SOURCE_H
#define ESP32_SERVER_DASHBOARD_DUMMY_SOURCE_H

#include <stdint.h>
#include "../Source.hpp"

class DummySource : public Source
{
public:
    DummySource();
    ~DummySource();

    void refresh(void);
};

#endif
