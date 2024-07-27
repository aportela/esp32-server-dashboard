#include <Arduino.h>
#include "FPSDebug.hpp"

uint64_t FPSDebug::frameCount = 0;
uint64_t FPSDebug::lastTime = 0;
uint16_t FPSDebug::fps = 0;

uint16_t FPSDebug::getFPS(void)
{
  return (FPSDebug::fps);
}

void FPSDebug::loop(void)
{
  uint64_t currentTime = millis();
  uint64_t elapsedTime = currentTime - FPSDebug::lastTime;
  if (elapsedTime >= 1000)
  {
    FPSDebug::fps = static_cast<uint16_t>(static_cast<float>(FPSDebug::frameCount) / (static_cast<float>(elapsedTime) / 1000.0));
    FPSDebug::frameCount = 0;
    FPSDebug::lastTime = currentTime;
  }
  FPSDebug::frameCount++;
}
