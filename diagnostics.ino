// Module for diagnostics and monitoring.

// Pin definitions for ESP32
const int VOLTAGE_PIN = 35; // ADC1_CH7
const int CURRENT_PIN = 32; // ADC1_CH4

// Diagnostic variables
float voltage = 0.0;
float current = 0.0;
float generatorFeedbackVoltage = 0.0;

void setupDiagnostics() {
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(CURRENT_PIN, INPUT);
  // The generator feedback pin is already set as INPUT in generator.ino
}

void loopDiagnostics() {
  // Read sensor values periodically
  static unsigned long lastDiagTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastDiagTime > 500) { // Read every 500ms
    lastDiagTime = currentTime;
    readSensors();
    readGeneratorFeedback();
  }
}

void readGeneratorFeedback() {
  int feedbackRaw = analogRead(ANALOG_FEEDBACK_PIN);
  // The conversion will depend on the expected voltage range and divider circuit.
  generatorFeedbackVoltage = feedbackRaw * (5.0 / 1023.0);
}

void readSensors() {
  // Read and convert voltage
  int voltageRaw = analogRead(VOLTAGE_PIN);
  // Conversion formula will depend on the voltage divider used.
  // Example: 5V input maps to 1023. If using a 3:1 divider for a 15V max,
  // voltage = voltageRaw * (5.0 / 1023.0) * 3;
  voltage = voltageRaw * (5.0 / 1023.0); // Simple mapping for now

  // Read and convert current
  int currentRaw = analogRead(CURRENT_PIN);
  // Conversion formula will depend on the sensor (e.g., ACS712).
  // Example for ACS712 5A: current = (currentRaw * (5.0 / 1023.0) - 2.5) / 0.185;
  current = currentRaw * (5.0 / 1023.0); // Simple mapping for now
}
