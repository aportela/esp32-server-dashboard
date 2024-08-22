#ifndef ESP32_SERVER_DASHBOARD_SCREEN_INTERFACE_H
#define ESP32_SERVER_DASHBOARD_SCREEN_INTERFACE_H

class IScreen
{
public:
    virtual ~IScreen() {}
    virtual bool refresh(bool force = false) = 0;
};

#endif // ESP32_SERVER_DASHBOARD_SCREEN_INTERFACE_H
