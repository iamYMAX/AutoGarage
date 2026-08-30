#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <Arduino.h>

enum DataMode : uint8_t {
    MODE_SIMULATION = 0,
    MODE_LOOPBACK   = 1,
    MODE_REAL       = 2,
    MODE_HYBRID     = 3
};

enum SensorSourceType : uint8_t {
    SOURCE_SIMULATION = 0,
    SOURCE_LOOPBACK   = 1,
    SOURCE_REAL       = 2
};

enum SensorQuality : uint8_t {
    QUALITY_VALID        = 0,
    QUALITY_INVALID      = 1,
    QUALITY_STALE        = 2,
    QUALITY_OUT_OF_RANGE = 3,
    QUALITY_NO_SIGNAL    = 4,
    QUALITY_SIMULATED    = 5
};

enum CrankPattern : uint8_t {
    PATTERN_60_2 = 0,
    PATTERN_36_1 = 1,
    PATTERN_36_2 = 2,
    PATTERN_12_1 = 3
};

struct SensorValue {
    uint64_t timestampUs;
    float value;
    uint8_t sensorId;
    SensorSourceType source;
    SensorQuality quality;
    uint32_t sequence;
};

struct EngineState {
    uint64_t timestampUs;
    float rpm;
    float crankAngle;
    float camAngle;
    float throttle;
    float map;
    float coolantTemp;
    float intakeTemp;
    float batteryVoltage;
    float lambda;
    float vehicleSpeed;
    float injectorPulseWidth;
    float ignitionAdvance;
    float ignitionDwell;
    bool crankSync;
    bool camSync;
    uint32_t cycle;
};

struct FaultGroundTruth {
    char faultId[32];
    float severity;
    uint64_t startTimeUs;
    uint64_t endTimeUs;
    bool active;
};

struct DiagnosticSession {
    char sessionId[36];
    uint64_t startTimeUs;
    uint8_t dataMode;
};

struct SystemState {
    DataMode globalDataMode;

    // Per-sensor sources for HYBRID mode
    SensorSourceType sourceCkp;
    SensorSourceType sourceCmp;
    SensorSourceType sourceTps;
    SensorSourceType sourceMap;
    SensorSourceType sourceEct;
    SensorSourceType sourceIat;
    SensorSourceType sourceBattery;
    SensorSourceType sourceCan;

    EngineState engine;
    FaultGroundTruth activeFault;
    DiagnosticSession session;

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
    uint32_t queueOverflowCount;
    bool sdCardReady;
    bool wifiConnected;
};

extern SystemState gSystemState;

#endif // SYSTEM_STATE_H
