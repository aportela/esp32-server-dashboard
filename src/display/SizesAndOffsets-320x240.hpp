#ifndef ESP32_SERVER_DASHBOARD_SCREEN_SIZES_AND_OFFSETS_H
#define ESP32_SERVER_DASHBOARD_SCREEN_SIZES_AND_OFFSETS_H

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

// ST7789 native resolution is 240x320 so we need (for LovyAnn driver) to invert width/height values & rotate
#define DISPLAY_DRIVER_LOVYANN_ST7789_WIDTH DISPLAY_HEIGHT
#define DISPLAY_DRIVER_LOVYANN_ST7789_HEIGHT DISPLAY_WIDTH
#define DISPLAY_DRIVER_LOVYANN_ST7789_ROTATION 3

#endif
