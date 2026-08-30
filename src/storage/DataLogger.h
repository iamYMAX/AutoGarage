#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "config/PinConfig.h"
#include "core/Event.h"
#include "core/EventQueue.h"
#include "core/SystemState.h"

class DataLogger {
private:
    bool sdInitialized;
    File logFile;

public:
    DataLogger();
    ~DataLogger();

    bool begin();
    void processEvents();
    void logEvent(const EngineEvent& ev);
};

extern DataLogger gDataLogger;

#endif // DATA_LOGGER_H
