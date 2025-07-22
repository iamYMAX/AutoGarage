// Module for handling the user menu and display.

#include <LiquidCrystal_I2C.h>

// Initialize the I2C display. Address 0x27, 16 columns, 2 rows.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Menu state variables
enum MenuState {
  MENU_RPM,
  MENU_GEN_MODE,
  // Add more menu items here
  MENU_ITEM_COUNT
};
MenuState currentMenu = MENU_RPM;


void setupMenu() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Engine Emulator");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
}

void loopMenu() {
  // Update the display with current data
  updateDisplay();
}

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);

  switch (currentMenu) {
    case MENU_RPM:
      lcd.print("> RPM: ");
      lcd.print(rpm);
      lcd.print("    ");
      break;
    case MENU_GEN_MODE:
      lcd.print("> Gen Mode: ");
      switch (currentGeneratorType) {
        case GEN_TYPE_PWM: lcd.print("PWM"); break;
        case GEN_TYPE_ON_OFF: lcd.print("ON/OFF"); break;
        case GEN_TYPE_CAN: lcd.print("CAN"); break;
      }
      break;
  }
}

// Functions to navigate the menu, called from encoder.ino
void menuNext() {
  currentMenu = (MenuState)(((int)currentMenu + 1) % MENU_ITEM_COUNT);
}

void menuPrev() {
  // This is handled by the encoder logic directly for now
}

void menuSelect() {
  // This is the action of pressing the encoder button
  menuNext(); // For now, just cycle through the menu
}
