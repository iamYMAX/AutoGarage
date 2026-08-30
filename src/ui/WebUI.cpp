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
                    if (doc.containsKey("sourceCkp")) gSystemState.sourceCkp = (SensorSourceType)((int)doc["sourceCkp"]);
                    if (doc.containsKey("sourceCmp")) gSystemState.sourceCmp = (SensorSourceType)((int)doc["sourceCmp"]);
                    if (doc.containsKey("sourceTps")) gSystemState.sourceTps = (SensorSourceType)((int)doc["sourceTps"]);
                    if (doc.containsKey("sourceMap")) gSystemState.sourceMap = (SensorSourceType)((int)doc["sourceMap"]);
                    if (doc.containsKey("sourceEct")) gSystemState.sourceEct = (SensorSourceType)((int)doc["sourceEct"]);
                    if (doc.containsKey("sourceIat")) gSystemState.sourceIat = (SensorSourceType)((int)doc["sourceIat"]);
                    if (doc.containsKey("sourceBattery")) gSystemState.sourceBattery = (SensorSourceType)((int)doc["sourceBattery"]);
                    if (doc.containsKey("sourceCan")) gSystemState.sourceCan = (SensorSourceType)((int)doc["sourceCan"]);
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
        StaticJsonDocument<768> doc;
        doc["dataMode"] = (uint8_t)gSystemState.globalDataMode;
        doc["rpm"] = gSystemState.currentRpm;
        doc["crankPattern"] = (uint8_t)gSystemState.crankPattern;
        doc["crankEnabled"] = gSystemState.crankEnabled;
        doc["batteryVoltage"] = gSystemState.batteryVoltage;
        doc["currentAmps"] = gSystemState.currentAmps;
        doc["canFramesCount"] = gSystemState.canFramesCount;
        doc["capturedEventsCount"] = gSystemState.capturedEventsCount;
        doc["queueOverflowCount"] = gSystemState.queueOverflowCount;
        doc["sdCardReady"] = gSystemState.sdCardReady;

        JsonObject engine = doc.createNestedObject("engine");
        engine["rpm"] = gSystemState.engine.rpm;
        engine["throttle"] = gSystemState.engine.throttle;
        engine["map"] = gSystemState.engine.map;
        engine["coolantTemp"] = gSystemState.engine.coolantTemp;
        engine["intakeTemp"] = gSystemState.engine.intakeTemp;
        engine["batteryVoltage"] = gSystemState.engine.batteryVoltage;
        engine["lambda"] = gSystemState.engine.lambda;
        engine["injectorPulseWidth"] = gSystemState.engine.injectorPulseWidth;
        engine["ignitionDwell"] = gSystemState.engine.ignitionDwell;

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

    StaticJsonDocument<384> doc;
    doc["dataMode"] = (uint8_t)gSystemState.globalDataMode;
    doc["rpm"] = gSystemState.currentRpm;
    doc["vBat"] = gSystemState.batteryVoltage;
    doc["can"] = gSystemState.canFramesCount;
    doc["events"] = gSystemState.capturedEventsCount;
    doc["overflows"] = gSystemState.queueOverflowCount;

    String msg;
    serializeJson(doc, msg);
    ws.textAll(msg);
}
