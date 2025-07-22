// Module for handling the user menu and display.

#include <LiquidCrystal_I2C.h>

// Initialize the I2C display. Address 0x27, 16 columns, 2 rows.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Advanced Menu Structure using structs

// Forward declaration for the struct
struct MenuItem;

// Define a function pointer type for menu actions
typedef void (*MenuAction)();

// Struct to represent a single menu item
struct MenuItem {
  const char* title;      // Text to display
  MenuItem* parent;       // Pointer to the parent menu item
  MenuItem* firstChild;   // Pointer to the first item in a sub-menu
  MenuItem* nextSibling;  // Pointer to the next item at the same level
  MenuAction action;      // Function to call when selected
};

// --- Action Function Prototypes ---
void enterEditMode(int* value, const char* title);
void actionSelectProfile();
void actionSelectGenType();
void actionEditRpm();
void actionEditGenDuty();
void actionToggleLogger();
void actionResetSettings();
void dummyAction();

// --- Menu Definition ---
// Declare all menu items
MenuItem mainMenu, profilesMenu, generatorMenu, crankMenu, injectorsMenu, canMenu, diagMenu, settingsMenu;
// Profiles
MenuItem profile1, profile2, profile3;
// Generator
MenuItem genType, genPwmDuty, genCanId;
MenuItem genTypePwm, genTypeOnOff, genTypeCan;
// Crank
MenuItem crankRpm, crankType, crankType60_2, crankType36_1;
// Injectors
MenuItem injCount, injSim, injClean;
// CAN/Logger
MenuItem loggerToggle, loggerIds, loggerStatus;
// Diagnostics
MenuItem diagGenVoltage, diagInjCurrent, diagCanResp, diagErrors;
// Settings
MenuItem settingsTime, settingsContrast, settingsLang, settingsReset;
MenuItem langRu, langEn;


// Define the full menu structure (root level)
MenuItem mainMenu =      {"Profiles",      nullptr,      &profilesMenu,   nullptr,         dummyAction};
  MenuItem profilesMenu =  {"Profiles",      &mainMenu,    &profile1,       &generatorMenu,  dummyAction};
  MenuItem generatorMenu = {"Generator",     &mainMenu,    &genType,        &crankMenu,      dummyAction};
  MenuItem crankMenu =     {"Crankshaft",    &mainMenu,    &crankRpm,       &injectorsMenu,  dummyAction};
  MenuItem injectorsMenu = {"Injectors",     &mainMenu,    &injCount,       &canMenu,        dummyAction};
  MenuItem canMenu =       {"CAN/Logger",    &mainMenu,    &loggerToggle,   &diagMenu,       dummyAction};
  MenuItem diagMenu =      {"Diagnostics",   &mainMenu,    &diagGenVoltage, &settingsMenu,   dummyAction};
  MenuItem settingsMenu =  {"Settings",      &mainMenu,    &settingsTime,   nullptr,         dummyAction};

// Define sub-menus
// Profiles
MenuItem profile1 =    {"- Petrol 4-cyl",  &profilesMenu,nullptr,         &profile2,       actionSelectProfile};
MenuItem profile2 =    {"- Diesel 6-cyl",  &profilesMenu,nullptr,         &profile3,       actionSelectProfile};
MenuItem profile3 =    {"- Custom",        &profilesMenu,nullptr,         nullptr,         actionSelectProfile};
// Generator
MenuItem genType =     {"- Type",        &generatorMenu,&genTypePwm,     &genPwmDuty,     dummyAction};
  MenuItem genTypePwm ={"-- PWM",         &genType,     nullptr,         &genTypeOnOff,   actionSelectGenType};
  MenuItem genTypeOnOff={"-- ON/OFF",     &genType,     nullptr,         &genTypeCan,     actionSelectGenType};
  MenuItem genTypeCan ={"-- CAN",         &genType,     nullptr,         nullptr,         actionSelectGenType};
MenuItem genPwmDuty =  {"- Duty Cycle",  &generatorMenu,nullptr,         &genCanId,       actionEditGenDuty};
MenuItem genCanId =    {"- CAN ID",      &generatorMenu,nullptr,         nullptr,         dummyAction};
// Crankshaft
MenuItem crankRpm =    {"- RPM",         &crankMenu,   nullptr,         &crankType,      actionEditRpm};
MenuItem crankType =   {"- Type",        &crankMenu,   &crankType60_2,  nullptr,         dummyAction};
  MenuItem crankType60_2={"-- 60-2",       &crankType,   nullptr,         &crankType36_1,  dummyAction};
  MenuItem crankType36_1={"-- 36-1",       &crankType,   nullptr,         nullptr,         dummyAction};
// Injectors
MenuItem injCount =    {"- Quantity",    &injectorsMenu,nullptr,         &injSim,         dummyAction};
MenuItem injSim =      {"- Simulate",    &injectorsMenu,nullptr,         &injClean,       dummyAction};
MenuItem injClean =    {"- Cleaning Mode", &injectorsMenu,nullptr,         nullptr,         dummyAction};
// CAN/Logger
MenuItem loggerToggle ={"- Logger",      &canMenu,     nullptr,         &loggerIds,      actionToggleLogger};
MenuItem loggerIds =   {"- Logged IDs",  &canMenu,     nullptr,         &loggerStatus,   dummyAction};
MenuItem loggerStatus ={"- SD Status",   &canMenu,     nullptr,         nullptr,         dummyAction};
// Diagnostics
MenuItem diagGenVoltage={"- Gen Voltage", &diagMenu,    nullptr,         &diagInjCurrent, dummyAction};
MenuItem diagInjCurrent={"- Inj Current", &diagMenu,    nullptr,         &diagCanResp,    dummyAction};
MenuItem diagCanResp = {"- CAN Response", &diagMenu,    nullptr,         &diagErrors,     dummyAction};
MenuItem diagErrors =  {"- Errors",      &diagMenu,    nullptr,         nullptr,         dummyAction};
// Settings
MenuItem settingsTime =  {"- Set Time",    &settingsMenu,nullptr,         &settingsContrast,dummyAction};
MenuItem settingsContrast={"- Contrast",    &settingsMenu,nullptr,         &settingsLang,   dummyAction};
MenuItem settingsLang =  {"- Language",    &settingsMenu,&langRu,         &settingsReset,  dummyAction};
  MenuItem langRu =      {"-- Russian",    &settingsLang,nullptr,         &langEn,         dummyAction};
  MenuItem langEn =      {"-- English",    &settingsLang,nullptr,         nullptr,         dummyAction};
MenuItem settingsReset={"- Reset All",   &settingsMenu,nullptr,         nullptr,         actionResetSettings};

// Pointer to the currently displayed menu
MenuItem* currentMenu = &mainMenu;

// --- Edit Mode Variables ---
bool isInEditMode = false;
int* valueToEdit = nullptr;
const char* editTitle = "";

void enterEditMode(int* value, const char* title) {
  isInEditMode = true;
  valueToEdit = value;
  editTitle = title;
}

void exitEditMode() {
  isInEditMode = false;
  valueToEdit = nullptr;
}


void setupMenu() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Emulator v2.0");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
}

void loopMenu() {
  // The display logic will be more complex now
  updateDisplay();
}

// --- Navigation and Display Logic ---

// Pointer to the first item visible on the screen (for scrolling)
MenuItem* topVisibleItem = &mainMenu;

void updateDisplay() {
  lcd.clear();

  if (isInEditMode) {
    lcd.setCursor(0, 0);
    lcd.print("Editing:");
    lcd.setCursor(0, 1);
    lcd.print(editTitle);
    lcd.setCursor(10, 1);
    lcd.print(*valueToEdit);
    lcd.print("    ");
  } else {
    MenuItem* item = topVisibleItem;
    int i = 0;
    while (item != nullptr && i < 2) { // Assuming a 16x2 display
      lcd.setCursor(0, i);
      if (item == currentMenu) {
        lcd.print(">");
      } else {
        lcd.print(" ");
      }
      lcd.print(item->title);
      item = item->nextSibling;
      i++;
    }
  }
}

void navigateNext() {
  if (currentMenu->nextSibling != nullptr) {
    currentMenu = currentMenu->nextSibling;
    // Scroll the view if necessary
    // A simple implementation: if we move off the bottom, scroll down.
    if (currentMenu != topVisibleItem && currentMenu != topVisibleItem->nextSibling) {
        topVisibleItem = currentMenu->parent->firstChild;
        while(topVisibleItem->nextSibling != currentMenu)
        {
            topVisibleItem = topVisibleItem->nextSibling;
        }
    }
  }
}

void navigatePrev() {
  if (currentMenu != topVisibleItem) {
      MenuItem* prev = topVisibleItem;
      while(prev->nextSibling != currentMenu)
      {
          prev = prev->nextSibling;
      }
      currentMenu = prev;
  }
}


void enterMenu() {
  if (currentMenu->firstChild != nullptr) {
    // Enter sub-menu
    currentMenu = currentMenu->firstChild;
    topVisibleItem = currentMenu;
  } else if (currentMenu->action != nullptr) {
    // Execute action
    currentMenu->action();
  }
}

void goBack() {
  if (currentMenu->parent != nullptr) {
    currentMenu = currentMenu->parent;
    topVisibleItem = currentMenu;
  }
}

void dummyAction() {
  // Does nothing. Used for parent menu items.
}

// --- Action Implementations ---
void actionSelectProfile() {
  // Logic to load a profile will go here
  // For now, just a placeholder message
  lcd.clear();
  lcd.print("Profile loaded!");
  delay(1000);
}

void actionSelectGenType() {
  // Logic to set the generator type
  if (currentMenu == &genTypePwm) setGeneratorType(GEN_TYPE_PWM);
  else if (currentMenu == &genTypeOnOff) setGeneratorType(GEN_TYPE_ON_OFF);
  else if (currentMenu == &genTypeCan) setGeneratorType(GEN_TYPE_CAN);
  goBack(); // Go back to the parent menu
}

void actionEditRpm() {
  enterEditMode(&rpm, "RPM");
}

void actionEditGenDuty() {
  enterEditMode(&pwmDutyCycle, "Duty Cycle");
}

void actionToggleLogger() {
  // Logic to toggle logger on/off
}

void actionResetSettings() {
  // Logic to reset all EEPROM settings
  lcd.clear();
  lcd.print("Settings Reset!");
  delay(1000);
}
