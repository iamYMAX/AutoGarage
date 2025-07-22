// Module for handling the rotary encoder and buttons.

#include "GyverEncoder.h"

// Pin definitions for the encoder
#define CLK_PIN 10
#define DT_PIN  11
#define SW_PIN  12

// Create an encoder object
Encoder enc(CLK_PIN, DT_PIN, SW_PIN);

void setupEncoder() {
  enc.setType(TYPE2); // Set encoder type (adjust if needed)
}

void loopEncoder() {
  enc.tick(); // Must be called in the main loop

  // --- Encoder Rotation ---
  if (enc.isRight()) {
    // Increase RPM
    rpm += 100;
    if (rpm > 8000) rpm = 8000;
    setRpm(rpm); // Update the crank module
  }
  if (enc.isLeft()) {
    // Decrease RPM
    rpm -= 100;
    if (rpm < 0) rpm = 0;
    setRpm(rpm); // Update the crank module
  }

  // --- Encoder Button Click ---
  if (enc.isClick()) {
    // Placeholder for menu selection action
    // menuSelect();
  }
}
