// Module for handling the rotary encoder and buttons.

#include "GyverEncoder.h"

// Pin definitions for the encoder on ESP32
#define CLK_PIN 16
#define DT_PIN  17
#define SW_PIN  21

// Create an encoder object
Encoder enc(CLK_PIN, DT_PIN, SW_PIN);

void setupEncoder() {
  enc.setType(TYPE2); // Set encoder type (adjust if needed)
}

void loopEncoder() {
  enc.tick();

  if (isInEditMode) {
    // --- In Edit Mode ---
    int change = 0;
    if (enc.isRight()) change = 1;
    if (enc.isLeft()) change = -1;

    if (change != 0) {
      *valueToEdit += change;
      // After changing the value, we need to call the appropriate update function.
      // This is a bit tricky without knowing which variable is being edited.
      // A more robust solution would use a callback function.
      // For now, we'll just call all of them.
      setRpm(rpm);
      setGeneratorDutyCycle(pwmDutyCycle);
    }

    if (enc.isClick()) {
      exitEditMode(); // Exit edit mode on click
    }
  } else {
    // --- In Navigation Mode ---
    if (enc.isRight()) {
      navigateNext();
    }
    if (enc.isLeft()) {
      navigatePrev();
    }
    if (enc.isClick()) {
      enterMenu();
    }
    if (enc.isHolded()) {
      goBack();
    }
  }
}
