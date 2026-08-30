#include "core/EventQueue.h"
#include "core/SystemState.h"
#include "core/Timestamp.h"

EventQueue globalEventQueue;

SystemState gSystemState = {
    MODE_SIMULATION,

    // Per-sensor sources
    SOURCE_SIMULATION,
    SOURCE_SIMULATION,
    SOURCE_SIMULATION,
    SOURCE_SIMULATION,
    SOURCE_SIMULATION,
    SOURCE_SIMULATION,
    SOURCE_SIMULATION,
    SOURCE_SIMULATION,

    // EngineState
    {
        0,        // timestampUs
        1000.0f,  // rpm
        0.0f,     // crankAngle
        0.0f,     // camAngle
        15.0f,    // throttle
        35.0f,    // map
        85.0f,    // coolantTemp
        25.0f,    // intakeTemp
        13.8f,    // batteryVoltage
        1.0f,     // lambda
        0.0f,     // vehicleSpeed
        3000.0f,  // injectorPulseWidth
        10.0f,    // ignitionAdvance
        3000.0f,  // ignitionDwell
        true,     // crankSync
        true,     // camSync
        0         // cycle
    },

    // FaultGroundTruth
    {
        "NONE",
        0.0f,
        0,
        0,
        false
    },

    // DiagnosticSession
    {
        "session_default",
        0,
        0
    },

    1000,          // targetRpm
    1000,          // currentRpm
    PATTERN_60_2,  // crankPattern
    true,          // crankEnabled
    true,          // cmpEnabled
    0.0f,          // cmpPhaseDegrees
    3000,          // injectorPulseWidthUs
    0,             // injectorMode
    3000,          // ignitionDwellUs
    10.0f,         // ignitionAdvanceDeg
    500000,        // canBitrate
    13.8f,         // batteryVoltage
    0.0f,          // currentAmps
    13.8f,         // generatorFbVoltage
    0,             // generatedTeethCount
    0,             // capturedEventsCount
    0,             // canFramesCount
    0,             // queueOverflowCount
    false,         // sdCardReady
    false          // wifiConnected
};

static uint32_t sensorSequence = 0;

void incrementQueueOverflowCount() {
    gSystemState.queueOverflowCount++;
}

SensorSourceType getResolvedSource(SensorSourceType specificSource) {
    if (gSystemState.globalDataMode == MODE_SIMULATION) return SOURCE_SIMULATION;
    if (gSystemState.globalDataMode == MODE_LOOPBACK) return SOURCE_LOOPBACK;
    if (gSystemState.globalDataMode == MODE_REAL) return SOURCE_REAL;
    return specificSource; // MODE_HYBRID
}

SensorValue processSensorReading(uint8_t sensorId, float rawValue, SensorSourceType specificSource) {
    SensorValue sv;
    sv.timestampUs = getMonotonicTimestampUs();
    sv.sensorId = sensorId;
    sv.sequence = ++sensorSequence;
    sv.source = getResolvedSource(specificSource);

    if (sv.source == SOURCE_SIMULATION) {
        sv.quality = QUALITY_SIMULATED;
    } else {
        sv.quality = (rawValue >= 0.0f) ? QUALITY_VALID : QUALITY_INVALID;
    }

    sv.value = rawValue;
    return sv;
}

void updateEngineStateFromSources() {
    uint64_t now = getMonotonicTimestampUs();
    gSystemState.engine.timestampUs = now;
    gSystemState.engine.rpm = (float)gSystemState.currentRpm;
    gSystemState.engine.batteryVoltage = gSystemState.batteryVoltage;
    gSystemState.engine.injectorPulseWidth = (float)gSystemState.injectorPulseWidthUs;
    gSystemState.engine.ignitionDwell = (float)gSystemState.ignitionDwellUs;
    gSystemState.engine.ignitionAdvance = gSystemState.ignitionAdvanceDeg;

    if (gSystemState.crankEnabled && gSystemState.generatedTeethCount > 0) {
        gSystemState.engine.crankSync = true;
    }
}
