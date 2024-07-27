#include <Arduino.h>
#include "FPS.hpp"

uint64_t FPS::frameCount = 0;
uint64_t FPS::lastTime = 0;
uint16_t FPS::fps = 0;

uint16_t FPS::getFPS(void)
{
  return (FPS::fps);
}

void FPS::loop(void)
{
  uint64_t currentTime = millis();
  uint64_t elapsedTime = currentTime - FPS::lastTime;
  if (elapsedTime >= 1000)
  {
    FPS::fps = static_cast<uint16_t>(static_cast<float>(FPS::frameCount) / (static_cast<float>(elapsedTime) / 1000.0));
    FPS::frameCount = 0;
    FPS::lastTime = currentTime;
  }
  FPS::frameCount++;
}
