#include "Injector.h"

Injector gInjector;

static const uint8_t INJECTOR_PINS_ARRAY[MAX_INJECTOR_CHANNELS] = {
    PIN_INJECTOR_1,
    PIN_INJECTOR_2,
    PIN_INJECTOR_3,
    PIN_INJECTOR_4,
    PIN_INJECTOR_5,
    PIN_INJECTOR_6
};

Injector::Injector() : numChannels(MAX_INJECTOR_CHANNELS) {}

Injector::~Injector() {}

bool Injector::begin() {
    for (uint8_t i = 0; i < numChannels; i++) {
        channels[i].pin = INJECTOR_PINS_ARRAY[i];
        channels[i].active = false;
        channels[i].turnOffTimeUs = 0;

        pinMode(channels[i].pin, OUTPUT);
        digitalWrite(channels[i].pin, LOW);
    }
    return true;
}

void Injector::triggerPulse(uint8_t channelBitmask, uint32_t durationUs) {
    uint64_t now = getMonotonicTimestampUs();
    uint64_t offTime = now + durationUs;

    for (uint8_t i = 0; i < numChannels; i++) {
        if (channelBitmask & (1 << i)) {
            digitalWrite(channels[i].pin, HIGH);
            channels[i].active = true;
            channels[i].turnOffTimeUs = offTime;

            EngineEvent ev;
            ev.timestamp_us = now;
            ev.event_type = EVENT_INJECTOR_ON;
            ev.channel = i + 1;
            ev.reserved = 0;
            ev.value = (int32_t)durationUs;
            globalEventQueue.push(ev);
        }
    }
}

void Injector::update() {
    uint64_t now = getMonotonicTimestampUs();

    for (uint8_t i = 0; i < numChannels; i++) {
        if (channels[i].active && now >= channels[i].turnOffTimeUs) {
            digitalWrite(channels[i].pin, LOW);
            channels[i].active = false;

            EngineEvent ev;
            ev.timestamp_us = now;
            ev.event_type = EVENT_INJECTOR_OFF;
            ev.channel = i + 1;
            ev.reserved = 0;
            ev.value = 0;
            globalEventQueue.push(ev);
        }
    }
}
