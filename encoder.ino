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
  if (enc.isRight() || enc.isLeft()) {
    bool isRight = enc.isRight();

    switch (currentMenu) {
      case MENU_RPM:
        rpm += isRight ? 100 : -100;
        if (rpm > 8000) rpm = 8000;
        if (rpm < 0) rpm = 0;
        setRpm(rpm);
        break;
      case MENU_GEN_MODE:
        int genTypeInt = (int)currentGeneratorType + (isRight ? 1 : -1);
        if (genTypeInt >= GEN_TYPE_CAN + 1) genTypeInt = 0;
        if (genTypeInt < 0) genTypeInt = GEN_TYPE_CAN;
        setGeneratorType((GeneratorType)genTypeInt);
        break;
    }
  }

  // --- Encoder Button Click ---
  if (enc.isClick()) {
    menuSelect(); // Go to the next menu item
  }
}
