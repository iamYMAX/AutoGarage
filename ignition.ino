// Module for controlling the ignition coil.

#define IGNITION_PIN 23 // A suitable GPIO pin on ESP32

// Ignition parameters
unsigned int dwellTime_us = 3000; // Dwell time in microseconds (3ms)
int ignitionTimingAngle = 10; // Ignition advance in degrees (placeholder)

void setupIgnition() {
  pinMode(IGNITION_PIN, OUTPUT);
  digitalWrite(IGNITION_PIN, LOW); // Coil is off by default
}

// Timing variable for non-blocking dwell control
unsigned long coilChargeEndTime_us = 0;
bool isCoilCharging = false;

// This function is called by the crankshaft module to START the ignition sequence.
void triggerIgnition() {
  if (!isCoilCharging) {
    digitalWrite(IGNITION_PIN, HIGH); // Start charging the coil
    coilChargeEndTime_us = micros() + dwellTime_us;
    isCoilCharging = true;
  }
}

// This function must be called in the main loop to handle the END of the dwell period.
void loopIgnition() {
  if (isCoilCharging && micros() >= coilChargeEndTime_us) {
    digitalWrite(IGNITION_PIN, LOW); // Fire the coil
    isCoilCharging = false;
  }
}

void setDwellTime(unsigned int dwell_ms) {
  // Convert milliseconds from menu to microseconds for internal use
  dwellTime_us = dwell_ms * 1000;
}
