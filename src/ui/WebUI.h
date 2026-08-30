#ifndef WEB_UI_H
#define WEB_UI_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "core/SystemState.h"
#include "crank/CrankGenerator.h"

class WebUI {
private:
    AsyncWebServer server;
    AsyncWebSocket ws;
    uint32_t lastWsBroadcastMs;

    void setupRoutes();

public:
    WebUI();
    ~WebUI();

    bool begin();
    void update();
    void broadcastState();
};

extern WebUI gWebUI;

#endif // WEB_UI_H
