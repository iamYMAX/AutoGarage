#include <Arduino.h>
#include "config/PinConfig.h"
#include "config/Config.h"
#include "config/ProfileManager.h"
#include "core/Event.h"
#include "core/EventQueue.h"
#include "core/SystemState.h"
#include "crank/CrankGenerator.h"
#include "capture/SignalCapture.h"
#include "can/CanInterface.h"
#include "injector/Injector.h"
#include "ignition/Ignition.h"
#include "adc/AdcManager.h"
#include "storage/DataLogger.h"
#include "ui/DisplayManager.h"
#include "ui/WebUI.h"
#include "selftest/SelfTest.h"

extern void updateEngineStateFromSources();

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n=== AutoGarage v2 Data Engine Node Starting ===");

    ProfileManager::begin();
    if (!ProfileManager::loadProfile(0)) {
        Serial.println("Using default system profile...");
    }

    gCrankGenerator.begin();
    gSignalCapture.begin();
    gCanInterface.begin(500000);
    gInjector.begin();
    gIgnition.begin();
    gAdcManager.begin();
    gDataLogger.begin();
    gDisplayManager.begin();
    gWebUI.begin();

    gCrankGenerator.start();

    Serial.println("=== AutoGarage v2 Data Engine Initialized ===");
    SelfTest::runSelfTest(Serial);
}

void loop() {
    updateEngineStateFromSources();

    gInjector.update();
    gIgnition.update();
    gCanInterface.poll();
    gAdcManager.update(100);
    gDataLogger.processEvents();
    gDisplayManager.update();
    gWebUI.update();

    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.equalsIgnoreCase("SELFTEST")) {
            SelfTest::runSelfTest(Serial);
        }
    }
}
