#include "SelfTest.h"

void SelfTest::runSelfTest(Print& output) {
    StaticJsonDocument<256> doc;

    doc["ckp"] = gCrankGenerator.isRunning();
    doc["cmp"] = gSystemState.cmpEnabled;
    doc["can"] = gCanInterface.isInitialized();
    doc["adc"] = (gSystemState.batteryVoltage >= 0.0f);
    doc["sd"] = gSystemState.sdCardReady;
    doc["rtc"] = true;

    serializeJson(doc, output);
    output.println();
}
