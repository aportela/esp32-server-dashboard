#ifndef ESP32_SERVER_DASHBOARD_SCREEN_INTERFACE_H
#define ESP32_SERVER_DASHBOARD_SCREEN_INTERFACE_H

enum appScreen
{
    APP_SCREEN_NONE,
    APP_SCREEN_INFO,
    APP_SCREEN_RESUME_DATA
};

class IScreen
{
public:
    virtual ~IScreen() {}
    virtual void refresh(bool force = false) = 0;
};

#endif
