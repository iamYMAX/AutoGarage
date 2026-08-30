#include "DisplayManager.h"

DisplayManager gDisplayManager;

DisplayManager::DisplayManager() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1), initialized(false), lastUpdateMs(0) {}

DisplayManager::~DisplayManager() {}

bool DisplayManager::begin() {
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        initialized = false;
        return false;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("AutoGarage v2.0");
    display.println("Data Collection Node");
    display.display();

    initialized = true;
    return true;
}

void DisplayManager::update() {
    if (!initialized) return;

    uint32_t nowMs = millis();
    if (nowMs - lastUpdateMs < 250) return;
    lastUpdateMs = nowMs;

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("AutoGarage v2");

    display.printf("RPM: %u\n", gSystemState.currentRpm);
    display.printf("Bat: %.2fV\n", gSystemState.batteryVoltage);
    display.printf("CAN: %u frames\n", gSystemState.canFramesCount);
    display.printf("SD: %s\n", gSystemState.sdCardReady ? "OK" : "NO SD");

    display.display();
}
