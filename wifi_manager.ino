#include <WiFi.h>
#include <WiFiClient.h>

// --- Wi-Fi Credentials ---
// In a real application, these would be managed by a library like WiFiManager
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// --- Access Point Settings ---
const char* ap_ssid = "EngineEmulator-Setup";
const char* ap_password = "password";

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect. Starting Access Point...");
    startAccessPoint();
  }
}

void startAccessPoint() {
  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loopWiFi() {
  // Optional: check connection status and reconnect if lost
}
