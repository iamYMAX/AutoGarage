#ifndef INJECTOR_H
#define INJECTOR_H

#include <Arduino.h>
#include "config/PinConfig.h"
#include "core/Event.h"
#include "core/EventQueue.h"
#include "core/Timestamp.h"
#include "core/SystemState.h"

#define MAX_INJECTOR_CHANNELS 6

struct InjectorChannel {
    uint8_t pin;
    bool active;
    uint64_t turnOffTimeUs;
};

class Injector {
private:
    InjectorChannel channels[MAX_INJECTOR_CHANNELS];
    uint8_t numChannels;

public:
    Injector();
    ~Injector();

    bool begin();
    void triggerPulse(uint8_t channelBitmask, uint32_t durationUs);
    void update();
};

extern Injector gInjector;

#endif // INJECTOR_H
