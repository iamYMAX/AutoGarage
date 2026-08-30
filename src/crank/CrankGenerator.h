#ifndef CRANK_GENERATOR_H
#define CRANK_GENERATOR_H

#include <Arduino.h>
#include <esp_timer.h>
#include "config/PinConfig.h"
#include "core/SystemState.h"

class CrankGenerator {
private:
    esp_timer_handle_t timerHandle;
    volatile bool running;

    uint16_t totalTeeth;
    uint16_t missingTeeth;
    uint32_t currentRpm;
    uint32_t halfToothPeriodUs;

    bool cmpEnabled;
    float cmpPhaseDeg;
    uint32_t cmpPulseWidthUs;

    volatile uint16_t currentTooth;
    volatile bool pinState;
    volatile bool cmpState;

    static void IRAM_ATTR timerCallback(void* arg);
    void IRAM_ATTR handleTimerTick();

public:
    CrankGenerator();
    ~CrankGenerator();

    bool begin();
    void setPattern(CrankPattern pattern);
    void setRpm(uint32_t rpm);
    void setCmpEnabled(bool enabled);
    void setCmpPhase(float degrees);
    void setCmpPulseWidthUs(uint32_t us);

    void start();
    void stop();

    bool isRunning() const { return running; }
    uint32_t getRpm() const { return currentRpm; }
};

extern CrankGenerator gCrankGenerator;

#endif // CRANK_GENERATOR_H
