// Module for generating the crankshaft position sensor signal.

// Pin definitions for ESP32
const int CRANK_SIGNAL_PIN = 27; // A suitable GPIO for PWM/pulse generation

// Crankshaft signal parameters
extern int rpm; // Defined in main.ino
extern int totalTeeth; // Defined in main.ino
extern int missingTeeth; // Defined in main.ino

// Timing variables
unsigned long lastCrankPulseTime = 0;
unsigned long crankPulseInterval = 0; // Time between teeth in microseconds

void setupCrank() {
  pinMode(CRANK_SIGNAL_PIN, OUTPUT);
  calculateCrankPulseInterval();
}

void loopCrank() {
  unsigned long currentTime = micros();
  if (currentTime - lastCrankPulseTime >= crankPulseInterval) {
    lastCrankPulseTime = currentTime;
    generateCrankSignal();
  }
}

void calculateCrankPulseInterval() {
  if (rpm > 0) {
    // Time for one revolution in microseconds
    unsigned long timePerRevolution = 60000000 / rpm;
    // Time per tooth
    crankPulseInterval = timePerRevolution / totalTeeth;
  } else {
    crankPulseInterval = 0;
  }
}

void generateCrankSignal() {
  static int toothCounter = 0;

  if (crankPulseInterval == 0) {
    digitalWrite(CRANK_SIGNAL_PIN, LOW);
    return;
  }

  // Trigger injectors at the beginning of the cycle (e.g., at tooth 1)
  if (toothCounter == 1) {
    triggerInjectors();
  }

  // Simulate the missing teeth gap
  if (toothCounter < (totalTeeth - missingTeeth)) {
    // Generate a pulse for a regular tooth
    digitalWrite(CRANK_SIGNAL_PIN, HIGH);
    delayMicroseconds(crankPulseInterval / 2); // 50% duty cycle
    digitalWrite(CRANK_SIGNAL_PIN, LOW);
  }
  // The gap happens for the duration of the missing teeth

  toothCounter++;

  if (toothCounter >= totalTeeth) {
    toothCounter = 0;
  }
}

void setRpm(int newRpm) {
  if (rpm != newRpm) {
    rpm = newRpm;
    calculateCrankPulseInterval();
    notifyClients(); // Notify web clients of the change
  }
}
