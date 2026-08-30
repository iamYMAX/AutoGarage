#ifndef ADC_MANAGER_H
#define ADC_MANAGER_H

#include <Arduino.h>
#include "config/PinConfig.h"
#include "core/Event.h"
#include "core/EventQueue.h"
#include "core/Timestamp.h"
#include "core/SystemState.h"

struct AdcChannelConfig {
    uint8_t pin;
    float dividerRatio;
    float offset;
    float scale;
};

class AdcManager {
private:
    AdcChannelConfig batteryConfig;
    AdcChannelConfig currentConfig;
    AdcChannelConfig generatorFbConfig;
    uint32_t lastSampleTimeMs;

public:
    AdcManager();
    ~AdcManager();

    bool begin();
    void update(uint32_t intervalMs = 100);

    float readVoltage(const AdcChannelConfig& config, int32_t& outRaw);
};

extern AdcManager gAdcManager;

#endif // ADC_MANAGER_H
