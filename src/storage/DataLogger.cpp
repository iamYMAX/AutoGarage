#include "DataLogger.h"

DataLogger gDataLogger;

DataLogger::DataLogger() : sdInitialized(false) {}

DataLogger::~DataLogger() {
    if (logFile) {
        logFile.close();
    }
}

bool DataLogger::begin() {
    if (SD.begin(PIN_SD_CS)) {
        sdInitialized = true;
        gSystemState.sdCardReady = true;
        logFile = SD.open("/engine_raw.jsonl", FILE_APPEND);
    } else {
        sdInitialized = false;
        gSystemState.sdCardReady = false;
    }
    return true;
}

void DataLogger::logEvent(const EngineEvent& ev) {
    StaticJsonDocument<128> doc;
    doc["t"] = ev.timestamp_us;
    doc["type"] = getEventTypeName(ev.event_type);
    doc["ch"] = ev.channel;
    doc["v"] = ev.value;

    serializeJson(doc, Serial);
    Serial.println();

    if (sdInitialized && logFile) {
        serializeJson(doc, logFile);
        logFile.println();
    }
}

void DataLogger::processEvents() {
    EngineEvent ev;
    while (globalEventQueue.pop(ev)) {
        logEvent(ev);
    }
}
