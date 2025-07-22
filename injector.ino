// Module for controlling the fuel injectors.

// Pin definitions
const int INJECTOR_PINS[] = {6, 7, 8, 9, 10, 11}; // Up to 6 injectors
const int NUM_INJECTORS = sizeof(INJECTOR_PINS) / sizeof(INJECTOR_PINS[0]);

// Injector parameters
extern int injectionMode; // Defined in main.ino
extern unsigned int pulseWidth; // Defined in main.ino

void setupInjectors() {
  for (int i = 0; i < NUM_INJECTORS; i++) {
    pinMode(INJECTOR_PINS[i], OUTPUT);
    digitalWrite(INJECTOR_PINS[i], LOW);
  }
}

void loopInjectors() {
  // The main logic for triggering injectors will be called from the crank loop
  // to ensure synchronization with the engine's rotation.
}

// This function will be called from the crank signal handler when an injection event should occur.
void triggerInjectors() {
  if (injectionMode == 1) { // All at once
    for (int i = 0; i < NUM_INJECTORS; i++) {
      digitalWrite(INJECTOR_PINS[i], HIGH);
    }
    delay(pulseWidth);
    for (int i = 0; i < NUM_INJECTORS; i++) {
      digitalWrite(INJECTOR_PINS[i], LOW);
    }
  } else { // Sequential (to be implemented)
    // Placeholder for sequential logic
  }
}

void setPulseWidth(unsigned int newWidth) {
  pulseWidth = newWidth;
}

void setInjectionMode(int mode) {
  injectionMode = mode;
}
