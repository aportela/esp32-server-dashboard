#ifndef ESP32_SERVER_DASHBOARD_FPS_H
#define ESP32_SERVER_DASHBOARD_FPS_H

#include <stdint.h>

class FPS
{
public:
  static void loop(void);
  static uint16_t getFPS(void);

private:
  static uint64_t frameCount;
  static uint64_t lastTime;
  static uint16_t fps;
};

#endif
