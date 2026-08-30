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

static uint32_t sensorSequenceCounter = 0;

void incrementQueueOverflowCount() {
    gSystemState.queueOverflowCount++;
}

SensorSourceType getResolvedSensorSource(SensorId sensor) {
    if (gSystemState.globalDataMode == MODE_SIMULATION) return SOURCE_SIMULATION;
    if (gSystemState.globalDataMode == MODE_LOOPBACK)   return SOURCE_LOOPBACK;
    if (gSystemState.globalDataMode == MODE_REAL)       return SOURCE_REAL;

    // MODE_HYBRID: Route per-sensor channel source
    switch (sensor) {
        case SENSOR_CKP:     return gSystemState.sourceCkp;
        case SENSOR_CMP:     return gSystemState.sourceCmp;
        case SENSOR_TPS:     return gSystemState.sourceTps;
        case SENSOR_MAP:     return gSystemState.sourceMap;
        case SENSOR_ECT:     return gSystemState.sourceEct;
        case SENSOR_IAT:     return gSystemState.sourceIat;
        case SENSOR_BATTERY: return gSystemState.sourceBattery;
        case SENSOR_CAN:     return gSystemState.sourceCan;
        default:             return SOURCE_SIMULATION;
    }
}

SensorQuality validateSensorReading(SensorId sensor, float value, SensorSourceType source) {
    if (source == SOURCE_SIMULATION) {
        return QUALITY_SIMULATED;
    }

    switch (sensor) {
        case SENSOR_CKP:
        case SENSOR_CMP:
            return (value >= 0.0f && value <= 10000.0f) ? QUALITY_VALID : QUALITY_OUT_OF_RANGE;
        case SENSOR_TPS:
            return (value >= 0.0f && value <= 100.0f) ? QUALITY_VALID : QUALITY_OUT_OF_RANGE;
        case SENSOR_MAP:
            return (value >= 0.0f && value <= 350.0f) ? QUALITY_VALID : QUALITY_OUT_OF_RANGE;
        case SENSOR_ECT:
        case SENSOR_IAT:
            // Allow negative temperatures down to -40°C
            return (value >= -40.0f && value <= 160.0f) ? QUALITY_VALID : QUALITY_OUT_OF_RANGE;
        case SENSOR_BATTERY:
            return (value >= 0.0f && value <= 30.0f) ? QUALITY_VALID : QUALITY_OUT_OF_RANGE;
        case SENSOR_LAMBDA:
            return (value >= 0.5f && value <= 2.5f) ? QUALITY_VALID : QUALITY_OUT_OF_RANGE;
        default:
            return QUALITY_VALID;
    }
}

SensorValue processSensorChannel(SensorId sensor, float rawValue, SensorSourceType rawSource) {
    SensorValue sv;
    sv.timestampUs = getMonotonicTimestampUs();
    sv.sensorId = (uint8_t)sensor;
    sv.sequence = ++sensorSequenceCounter;
    sv.source = getResolvedSensorSource(sensor);
    sv.quality = validateSensorReading(sensor, rawValue, sv.source);
    sv.value = rawValue;

    return sv;
}

void updateEngineStateFromSources() {
    uint64_t now = getMonotonicTimestampUs();
    gSystemState.engine.timestampUs = now;

    // Process all 11 sensor channels via Data Engine
    SensorValue svRpm     = processSensorChannel(SENSOR_CKP, (float)gSystemState.currentRpm, SOURCE_SIMULATION);
    SensorValue svBat     = processSensorChannel(SENSOR_BATTERY, gSystemState.batteryVoltage, SOURCE_REAL);
    SensorValue svTps     = processSensorChannel(SENSOR_TPS, gSystemState.engine.throttle, SOURCE_SIMULATION);
    SensorValue svMap     = processSensorChannel(SENSOR_MAP, gSystemState.engine.map, SOURCE_SIMULATION);
    SensorValue svEct     = processSensorChannel(SENSOR_ECT, gSystemState.engine.coolantTemp, SOURCE_SIMULATION);
    SensorValue svIat     = processSensorChannel(SENSOR_IAT, gSystemState.engine.intakeTemp, SOURCE_SIMULATION);
    SensorValue svLambda  = processSensorChannel(SENSOR_LAMBDA, gSystemState.engine.lambda, SOURCE_SIMULATION);
    SensorValue svInj     = processSensorChannel(SENSOR_INJ, (float)gSystemState.injectorPulseWidthUs, SOURCE_SIMULATION);
    SensorValue svIgnDwell= processSensorChannel(SENSOR_IGN, (float)gSystemState.ignitionDwellUs, SOURCE_SIMULATION);

    gSystemState.engine.rpm = svRpm.value;
    gSystemState.engine.batteryVoltage = svBat.value;
    gSystemState.engine.throttle = svTps.value;
    gSystemState.engine.map = svMap.value;
    gSystemState.engine.coolantTemp = svEct.value;
    gSystemState.engine.intakeTemp = svIat.value;
    gSystemState.engine.lambda = svLambda.value;
    gSystemState.engine.injectorPulseWidth = svInj.value;
    gSystemState.engine.ignitionDwell = svIgnDwell.value;
    gSystemState.engine.ignitionAdvance = gSystemState.ignitionAdvanceDeg;

    if (gSystemState.crankEnabled && gSystemState.generatedTeethCount > 0) {
        gSystemState.engine.crankSync = true;
        gSystemState.engine.camSync = gSystemState.cmpEnabled;
    }
}
