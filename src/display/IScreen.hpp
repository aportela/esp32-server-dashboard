#ifndef ESP32_SERVER_DASHBOARD_SCREEN_INTERFACE_H
#define ESP32_SERVER_DASHBOARD_SCREEN_INTERFACE_H

class IScreen
{
public:
    virtual ~IScreen() {}
    virtual void refresh(void) = 0;
};

#endif
