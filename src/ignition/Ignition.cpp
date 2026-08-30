#include "Ignition.h"

Ignition gIgnition;

Ignition::Ignition() : dwellTimeUs(3000), advanceDeg(10.0f), charging(false), fireTimeUs(0) {}

Ignition::~Ignition() {}

bool Ignition::begin() {
    pinMode(PIN_IGNITION_COIL, OUTPUT);
    digitalWrite(PIN_IGNITION_COIL, LOW);
    gSystemState.ignitionDwellUs = dwellTimeUs;
    gSystemState.ignitionAdvanceDeg = advanceDeg;
    return true;
}

void Ignition::setDwellTimeUs(uint32_t us) {
    dwellTimeUs = us;
    gSystemState.ignitionDwellUs = us;
}

void Ignition::setAdvanceDeg(float deg) {
    advanceDeg = deg;
    gSystemState.ignitionAdvanceDeg = deg;
}

void Ignition::startDwell(uint32_t dwellUs) {
    if (dwellUs > 0) dwellTimeUs = dwellUs;

    uint64_t now = getMonotonicTimestampUs();
    fireTimeUs = now + dwellTimeUs;
    charging = true;

    digitalWrite(PIN_IGNITION_COIL, HIGH);

    EngineEvent ev;
    ev.timestamp_us = now;
    ev.event_type = EVENT_IGNITION_DWELL_START;
    ev.channel = 1;
    ev.reserved = 0;
    ev.value = (int32_t)dwellTimeUs;
    globalEventQueue.push(ev);
}

void Ignition::update() {
    if (!charging) return;

    uint64_t now = getMonotonicTimestampUs();
    if (now >= fireTimeUs) {
        digitalWrite(PIN_IGNITION_COIL, LOW);
        charging = false;

        EngineEvent ev;
        ev.timestamp_us = now;
        ev.event_type = EVENT_IGNITION_FIRE;
        ev.channel = 1;
        ev.reserved = 0;
        ev.value = 0;
        globalEventQueue.push(ev);
    }
}
