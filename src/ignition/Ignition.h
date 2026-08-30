#ifndef IGNITION_H
#define IGNITION_H

#include <Arduino.h>
#include "config/PinConfig.h"
#include "core/Event.h"
#include "core/EventQueue.h"
#include "core/Timestamp.h"
#include "core/SystemState.h"

class Ignition {
private:
    uint32_t dwellTimeUs;
    float advanceDeg;
    bool charging;
    uint64_t fireTimeUs;

public:
    Ignition();
    ~Ignition();

    bool begin();
    void startDwell(uint32_t dwellUs = 3000);
    void update();

    void setDwellTimeUs(uint32_t us);
    void setAdvanceDeg(float deg);
};

extern Ignition gIgnition;

#endif // IGNITION_H
