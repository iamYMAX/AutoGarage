#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <Arduino.h>

enum CrankPattern : uint8_t {
    PATTERN_60_2 = 0,
    PATTERN_36_1 = 1,
    PATTERN_36_2 = 2,
    PATTERN_12_1 = 3
};

struct SystemState {
    uint32_t targetRpm;
    uint32_t currentRpm;
    CrankPattern crankPattern;
    bool crankEnabled;
    bool cmpEnabled;
    float cmpPhaseDegrees;

    uint32_t injectorPulseWidthUs;
    uint8_t injectorMode;

    uint32_t ignitionDwellUs;
    float ignitionAdvanceDeg;

    uint32_t canBitrate;

    float batteryVoltage;
    float currentAmps;
    float generatorFbVoltage;

    uint32_t generatedTeethCount;
    uint32_t capturedEventsCount;
    uint32_t canFramesCount;
    bool sdCardReady;
    bool wifiConnected;
};

extern SystemState gSystemState;

#endif // SYSTEM_STATE_H
