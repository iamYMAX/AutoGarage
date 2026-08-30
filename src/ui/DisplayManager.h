#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config/PinConfig.h"
#include "core/SystemState.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

class DisplayManager {
private:
    Adafruit_SSD1306 display;
    bool initialized;
    uint32_t lastUpdateMs;

public:
    DisplayManager();
    ~DisplayManager();

    bool begin();
    void update();
};

extern DisplayManager gDisplayManager;

#endif // DISPLAY_MANAGER_H
