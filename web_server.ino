#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
// Create a WebSocket object
AsyncWebSocket ws("/ws");

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  // Handle WebSocket events
  if(type == WS_EVT_CONNECT){
    Serial.println("WebSocket client connected");
    // Send a welcome message
    client->text("Welcome!");
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("WebSocket client disconnected");
  } else if(type == WS_EVT_DATA){
    // Handle data received from the client
    Serial.println("Data received");
  }
}

void setupWebServer() {
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Attach WebSocket event handler
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to load script.js file
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  // API route to get current status
  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    // Create a JSON document
    StaticJsonDocument<256> doc;
    doc["rpm"] = rpm;
    doc["gen_mode"] = currentGeneratorType; // This will be an int, need to convert to string on client
    doc["gen_duty"] = pwmDutyCycle;

    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
  });

  // API route to update settings
  server.on("/api/settings", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    StaticJsonDocument<256> doc;
    deserializeJson(doc, (const char*) data);

    if (doc.containsKey("rpm")) {
      rpm = doc["rpm"].as<int>();
      setRpm(rpm);
    }
    if (doc.containsKey("gen_duty")) {
      pwmDutyCycle = doc["gen_duty"].as<int>();
      setGeneratorDutyCycle(pwmDutyCycle);
    }

    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
  Serial.println("Web server started");
}

// Function to send status updates to all connected WebSocket clients
void notifyClients() {
  // Create a JSON document with the current status
  StaticJsonDocument<256> doc;
  doc["rpm"] = rpm;

  const char* genModeStr = "";
  switch (currentGeneratorType) {
    case GEN_TYPE_PWM: genModeStr = "PWM"; break;
    case GEN_TYPE_ON_OFF: genModeStr = "ON/OFF"; break;
    case GEN_TYPE_CAN: genModeStr = "CAN"; break;
  }
  doc["gen_mode"] = genModeStr;
  doc["gen_duty"] = pwmDutyCycle;

  String output;
  serializeJson(doc, output);

  // Send the JSON string to all clients
  ws.textAll(output);
}

void loopWebServer() {
  // The web server runs asynchronously. We just need to clean up clients.
  ws.cleanupClients();
}
