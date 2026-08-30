#include "CrankGenerator.h"

CrankGenerator gCrankGenerator;

CrankGenerator::CrankGenerator()
    : timerHandle(nullptr), running(false), totalTeeth(60), missingTeeth(2),
      currentRpm(1000), halfToothPeriodUs(500), cmpEnabled(true), cmpPhaseDeg(0.0f),
      cmpPulseWidthUs(1000), currentTooth(0), pinState(false), cmpState(false) {}

CrankGenerator::~CrankGenerator() {
    stop();
}

bool CrankGenerator::begin() {
    pinMode(PIN_CKP_OUT, OUTPUT);
    digitalWrite(PIN_CKP_OUT, LOW);

    pinMode(PIN_CMP_OUT, OUTPUT);
    digitalWrite(PIN_CMP_OUT, LOW);

    setPattern(PATTERN_60_2);
    setRpm(1000);

    esp_timer_create_args_t timerArgs = {};
    timerArgs.callback = &CrankGenerator::timerCallback;
    timerArgs.arg = this;
    timerArgs.name = "crank_timer";
    timerArgs.dispatch_method = ESP_TIMER_TASK;

    if (esp_timer_create(&timerArgs, &timerHandle) != ESP_OK) {
        return false;
    }

    return true;
}

void CrankGenerator::setPattern(CrankPattern pattern) {
    switch (pattern) {
        case PATTERN_60_2:
            totalTeeth = 60;
            missingTeeth = 2;
            break;
        case PATTERN_36_1:
            totalTeeth = 36;
            missingTeeth = 1;
            break;
        case PATTERN_36_2:
            totalTeeth = 36;
            missingTeeth = 2;
            break;
        case PATTERN_12_1:
            totalTeeth = 12;
            missingTeeth = 1;
            break;
        default:
            totalTeeth = 60;
            missingTeeth = 2;
            break;
    }
    gSystemState.crankPattern = pattern;
    setRpm(currentRpm);
}

void CrankGenerator::setRpm(uint32_t rpm) {
    if (rpm == 0) rpm = 1;
    currentRpm = rpm;
    gSystemState.targetRpm = rpm;
    gSystemState.currentRpm = rpm;

    uint64_t revUs = 60000000ULL / rpm;
    uint32_t toothPeriodUs = revUs / totalTeeth;
    halfToothPeriodUs = toothPeriodUs / 2;
    if (halfToothPeriodUs < 10) halfToothPeriodUs = 10;

    if (running && timerHandle) {
        esp_timer_stop(timerHandle);
        esp_timer_start_periodic(timerHandle, halfToothPeriodUs);
    }
}

void CrankGenerator::setCmpEnabled(bool enabled) {
    cmpEnabled = enabled;
    gSystemState.cmpEnabled = enabled;
}

void CrankGenerator::setCmpPhase(float degrees) {
    cmpPhaseDeg = degrees;
    gSystemState.cmpPhaseDegrees = degrees;
}

void CrankGenerator::setCmpPulseWidthUs(uint32_t us) {
    cmpPulseWidthUs = us;
}

void CrankGenerator::start() {
    if (running || !timerHandle) return;
    running = true;
    currentTooth = 0;
    pinState = false;
    cmpState = false;
    gSystemState.crankEnabled = true;
    esp_timer_start_periodic(timerHandle, halfToothPeriodUs);
}

void CrankGenerator::stop() {
    if (!running || !timerHandle) return;
    running = false;
    gSystemState.crankEnabled = false;
    esp_timer_stop(timerHandle);
    digitalWrite(PIN_CKP_OUT, LOW);
    digitalWrite(PIN_CMP_OUT, LOW);
}

void IRAM_ATTR CrankGenerator::timerCallback(void* arg) {
    static_cast<CrankGenerator*>(arg)->handleTimerTick();
}

void IRAM_ATTR CrankGenerator::handleTimerTick() {
    pinState = !pinState;

    if (pinState) {
        currentTooth++;
        if (currentTooth >= totalTeeth) {
            currentTooth = 0;
        }

        if (currentTooth < (totalTeeth - missingTeeth)) {
            digitalWrite(PIN_CKP_OUT, HIGH);
        } else {
            digitalWrite(PIN_CKP_OUT, LOW);
        }

        if (cmpEnabled) {
            if (currentTooth == 0) {
                digitalWrite(PIN_CMP_OUT, HIGH);
                cmpState = true;
            } else if (currentTooth == 2 && cmpState) {
                digitalWrite(PIN_CMP_OUT, LOW);
                cmpState = false;
            }
        }
    } else {
        digitalWrite(PIN_CKP_OUT, LOW);
    }

    gSystemState.generatedTeethCount++;
}
