// Module for handling the user menu and display.

#include <LiquidCrystal_I2C.h>

// Initialize the I2C display. Address 0x27, 16 columns, 2 rows.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Menu state variables
int currentMenu = 0;

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
  // For now, just display the RPM.
  // This will be expanded to show the full menu.
  lcd.setCursor(0, 0);
  lcd.print("RPM: ");
  lcd.print(rpm); // Assumes 'rpm' is a global variable from crank.ino
  lcd.print("    "); // Clear previous digits

  lcd.setCursor(0, 1);
  lcd.print("Mode: Crank Gen");
}

// Stubs for menu navigation functions
void menuNext() {}
void menuPrev() {}
void menuSelect() {}
