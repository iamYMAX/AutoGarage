#ifndef SELFTEST_H
#define SELFTEST_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "config/PinConfig.h"
#include "core/SystemState.h"
#include "crank/CrankGenerator.h"
#include "can/CanInterface.h"

class SelfTest {
public:
    static void runSelfTest(Print& output);
};

#endif // SELFTEST_H
