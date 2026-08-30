#include "core/EventQueue.h"
#include "core/SystemState.h"
#include "core/Timestamp.h"

EventQueue globalEventQueue;
SystemState gSystemState = {
    .globalDataMode = MODE_SIMULATION,
    .sourceCkp = SOURCE_SIMULATION,
    .sourceCmp = SOURCE_SIMULATION,
    .sourceTps = SOURCE_SIMULATION,
    .sourceMap = SOURCE_SIMULATION,
    .sourceEct = SOURCE_SIMULATION,
    .sourceIat = SOURCE_SIMULATION,
    .sourceBattery = SOURCE_SIMULATION,
    .sourceCan = SOURCE_SIMULATION,
    .engine = {
        .timestampUs = 0,
        .rpm = 1000.0f,
        .crankAngle = 0.0f,
        .camAngle = 0.0f,
        .throttle = 15.0f,
        .map = 35.0f,
        .coolantTemp = 85.0f,
        .intakeTemp = 25.0f,
        .batteryVoltage = 13.8f,
        .lambda = 1.0f,
        .vehicleSpeed = 0.0f,
        .injectorPulseWidth = 3000.0f,
        .ignitionAdvance = 10.0f,
        .ignitionDwell = 3000.0f,
        .crankSync = true,
        .camSync = true,
        .cycle = 0
    },
    .targetRpm = 1000,
    .currentRpm = 1000,
    .crankPattern = PATTERN_60_2,
    .crankEnabled = true,
    .cmpEnabled = true,
    .cmpPhaseDegrees = 0.0f,
    .injectorPulseWidthUs = 3000,
    .injectorMode = 0,
    .ignitionDwellUs = 3000,
    .ignitionAdvanceDeg = 10.0f,
    .canBitrate = 500000,
    .batteryVoltage = 13.8f,
    .currentAmps = 0.0f,
    .generatorFbVoltage = 13.8f,
    .generatedTeethCount = 0,
    .capturedEventsCount = 0,
    .canFramesCount = 0,
    .sdCardReady = false,
    .wifiConnected = false
};

void updateEngineStateFromSources() {
    uint64_t now = getMonotonicTimestampUs();
    gSystemState.engine.timestampUs = now;
    gSystemState.engine.rpm = (float)gSystemState.currentRpm;
    gSystemState.engine.batteryVoltage = gSystemState.batteryVoltage;
}
