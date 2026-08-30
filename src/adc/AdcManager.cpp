#include "AdcManager.h"

AdcManager gAdcManager;

AdcManager::AdcManager() : lastSampleTimeMs(0) {
    batteryConfig = {PIN_ADC_BATTERY, 11.0f, 0.0f, (3.3f / 4095.0f)};
    currentConfig = {PIN_ADC_CURRENT, 1.0f, 1.65f, (3.3f / 4095.0f)};
    generatorFbConfig = {PIN_ADC_GEN_FB, 5.0f, 0.0f, (3.3f / 4095.0f)};
}

AdcManager::~AdcManager() {}

bool AdcManager::begin() {
    analogReadResolution(12);
    pinMode(PIN_ADC_BATTERY, INPUT);
    pinMode(PIN_ADC_CURRENT, INPUT);
    pinMode(PIN_ADC_GEN_FB, INPUT);
    return true;
}

float AdcManager::readVoltage(const AdcChannelConfig& config, int32_t& outRaw) {
    outRaw = analogRead(config.pin);
    float pinVolt = outRaw * config.scale;
    float calibratedValue = (pinVolt - config.offset) * config.dividerRatio;
    return calibratedValue;
}

void AdcManager::update(uint32_t intervalMs) {
    uint32_t nowMs = millis();
    if (nowMs - lastSampleTimeMs < intervalMs) return;
    lastSampleTimeMs = nowMs;

    int32_t batRaw = 0, curRaw = 0, genRaw = 0;
    float batVal = readVoltage(batteryConfig, batRaw);
    float curVal = readVoltage(currentConfig, curRaw);
    float genVal = readVoltage(generatorFbConfig, genRaw);

    // Process ADC reading through Data Engine
    SensorValue svBat = processSensorChannel(SENSOR_BATTERY, batVal, SOURCE_REAL);

    gSystemState.batteryVoltage = svBat.value;
    gSystemState.currentAmps = curVal;
    gSystemState.generatorFbVoltage = genVal;

    EngineEvent ev;
    ev.timestamp_us = svBat.timestampUs;
    ev.event_type = EVENT_ADC_SAMPLE;
    ev.channel = (uint8_t)SENSOR_BATTERY;
    ev.reserved = (uint16_t)batRaw;
    ev.value = (int32_t)(svBat.value * 100.0f);
    globalEventQueue.push(ev);
}
