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
  enc.tick();

  if (isInEditMode) {
    // --- In Edit Mode ---
    if (enc.isRight()) {
      (*valueToEdit)++; // Increment the value
    }
    if (enc.isLeft()) {
      (*valueToEdit)--; // Decrement the value
    }
    if (enc.isClick()) {
      exitEditMode(); // Exit edit mode on click
      // Here you might want to call the function to update the hardware
      // e.g., setRpm(rpm);
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
