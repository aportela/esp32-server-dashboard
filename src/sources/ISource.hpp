#ifndef ESP32_SERVER_DASHBOARD_SOURCE_INTERFACE
#define ESP32_SERVER_DASHBOARD_SOURCE_INTERFACE

#include <stdint.h>
#include "SourceData.hpp"

class ISource
{
public:
    virtual ~ISource() {}
    virtual SourceData getCurrent(SourceDataType entity) = 0;
};

#endif
