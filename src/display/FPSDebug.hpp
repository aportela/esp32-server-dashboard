#ifndef ESP32_SERVER_DASHBOARD_FPS_H
#define ESP32_SERVER_DASHBOARD_FPS_H

#include <stdint.h>

class FPSDebug
{
public:
  ~FPSDebug() {}
  void loop(void);
  uint16_t getFPS(void);

private:
  uint64_t frameCount = 0;
  uint64_t lastTime = 0;
  uint16_t fps = 0;
};

#endif
