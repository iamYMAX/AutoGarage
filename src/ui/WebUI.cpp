#include "WebUI.h"

WebUI gWebUI;

WebUI::WebUI() : server(80), ws("/ws"), lastWsBroadcastMs(0) {}

WebUI::~WebUI() {}

bool WebUI::begin() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("AutoGarage-v2-Node", "password123");
    gSystemState.wifiConnected = true;

    LittleFS.begin(true);

    ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        if (type == WS_EVT_DATA) {
            AwsFrameInfo *info = (AwsFrameInfo*)arg;
            if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
                data[len] = 0;
                StaticJsonDocument<256> doc;
                DeserializationError error = deserializeJson(doc, (char*)data);
                if (!error) {
                    if (doc.containsKey("rpm")) {
                        uint32_t newRpm = doc["rpm"];
                        gCrankGenerator.setRpm(newRpm);
                    }
                    if (doc.containsKey("dataMode")) {
                        gSystemState.globalDataMode = (DataMode)((int)doc["dataMode"]);
                    }
                }
            }
        }
    });

    server.addHandler(&ws);
    setupRoutes();
    server.begin();

    return true;
}

void WebUI::setupRoutes() {
    server.on("/api/state", HTTP_GET, [](AsyncWebServerRequest *request) {
        StaticJsonDocument<512> doc;
        doc["dataMode"] = (uint8_t)gSystemState.globalDataMode;
        doc["rpm"] = gSystemState.currentRpm;
        doc["crankPattern"] = (uint8_t)gSystemState.crankPattern;
        doc["crankEnabled"] = gSystemState.crankEnabled;
        doc["batteryVoltage"] = gSystemState.batteryVoltage;
        doc["currentAmps"] = gSystemState.currentAmps;
        doc["canFramesCount"] = gSystemState.canFramesCount;
        doc["capturedEventsCount"] = gSystemState.capturedEventsCount;
        doc["sdCardReady"] = gSystemState.sdCardReady;

        JsonObject sources = doc.createNestedObject("sources");
        sources["ckp"] = (uint8_t)gSystemState.sourceCkp;
        sources["cmp"] = (uint8_t)gSystemState.sourceCmp;
        sources["tps"] = (uint8_t)gSystemState.sourceTps;
        sources["map"] = (uint8_t)gSystemState.sourceMap;
        sources["ect"] = (uint8_t)gSystemState.sourceEct;
        sources["iat"] = (uint8_t)gSystemState.sourceIat;
        sources["battery"] = (uint8_t)gSystemState.sourceBattery;
        sources["can"] = (uint8_t)gSystemState.sourceCan;

        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });

    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
}

void WebUI::update() {
    ws.cleanupClients();
    uint32_t nowMs = millis();
    if (nowMs - lastWsBroadcastMs >= 500) {
        lastWsBroadcastMs = nowMs;
        broadcastState();
    }
}

void WebUI::broadcastState() {
    if (ws.count() == 0) return;

    StaticJsonDocument<256> doc;
    doc["dataMode"] = (uint8_t)gSystemState.globalDataMode;
    doc["rpm"] = gSystemState.currentRpm;
    doc["vBat"] = gSystemState.batteryVoltage;
    doc["can"] = gSystemState.canFramesCount;
    doc["events"] = gSystemState.capturedEventsCount;

    String msg;
    serializeJson(doc, msg);
    ws.textAll(msg);
}
