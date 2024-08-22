#ifndef APORTELA_MICROCONTROLLER_UTILS_FPS_H
#define APORTELA_MICROCONTROLLER_UTILS_FPS_H

#include <stdint.h>

namespace aportela::microcontroller::utils
{

  /*
    WARNING: THIS IS NOT A "REAL FPS DATA", ONLY AN APROXIMATTION DATA FOR BENCH & OPTIMIZE CODE FOR MAX SCREEN UPDATES
  */
  class FPS
  {
  public:
    static void init(void);
    static void loop(uint16_t limit = 0);
    static uint16_t getFPS(void);

  private:
    static uint64_t frameCount;
    static uint64_t lastTime;
    static uint16_t fps;
  };
}

#endif // APORTELA_MICROCONTROLLER_UTILS_FPS_H
