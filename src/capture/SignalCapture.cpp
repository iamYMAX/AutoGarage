#include "SignalCapture.h"

SignalCapture gSignalCapture;

SignalCapture::SignalCapture() : enabled(false) {}

SignalCapture::~SignalCapture() {
    stop();
}

bool SignalCapture::begin() {
    pinMode(PIN_CKP_IN, INPUT);
    pinMode(PIN_CMP_IN, INPUT);

    attachInterrupt(digitalPinToInterrupt(PIN_CKP_IN), &SignalCapture::onCkpRisingIsr, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_CKP_IN), &SignalCapture::onCkpFallingIsr, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_CMP_IN), &SignalCapture::onCmpRisingIsr, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_CMP_IN), &SignalCapture::onCmpFallingIsr, FALLING);

    enabled = true;
    return true;
}

void SignalCapture::stop() {
    if (!enabled) return;
    detachInterrupt(digitalPinToInterrupt(PIN_CKP_IN));
    detachInterrupt(digitalPinToInterrupt(PIN_CMP_IN));
    enabled = false;
}

void IRAM_ATTR SignalCapture::onCkpRisingIsr() {
    uint64_t ts = getMonotonicTimestampUs();
    EngineEvent ev;
    ev.timestamp_us = ts;
    ev.event_type = EVENT_CKP_RISING;
    ev.channel = 0;
    ev.reserved = 0;
    ev.value = 0;

    globalEventQueue.push(ev);
    gSystemState.capturedEventsCount++;
}

void IRAM_ATTR SignalCapture::onCkpFallingIsr() {
    uint64_t ts = getMonotonicTimestampUs();
    EngineEvent ev;
    ev.timestamp_us = ts;
    ev.event_type = EVENT_CKP_FALLING;
    ev.channel = 0;
    ev.reserved = 0;
    ev.value = 0;

    globalEventQueue.push(ev);
    gSystemState.capturedEventsCount++;
}

void IRAM_ATTR SignalCapture::onCmpRisingIsr() {
    uint64_t ts = getMonotonicTimestampUs();
    EngineEvent ev;
    ev.timestamp_us = ts;
    ev.event_type = EVENT_CMP_RISING;
    ev.channel = 1;
    ev.reserved = 0;
    ev.value = 0;

    globalEventQueue.push(ev);
    gSystemState.capturedEventsCount++;
}

void IRAM_ATTR SignalCapture::onCmpFallingIsr() {
    uint64_t ts = getMonotonicTimestampUs();
    EngineEvent ev;
    ev.timestamp_us = ts;
    ev.event_type = EVENT_CMP_FALLING;
    ev.channel = 1;
    ev.reserved = 0;
    ev.value = 0;

    globalEventQueue.push(ev);
    gSystemState.capturedEventsCount++;
}
