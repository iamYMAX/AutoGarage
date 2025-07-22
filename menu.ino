// Module for handling the user menu and display.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
void actionEditDwellTime();
void actionToggleLogger();
void actionResetSettings();
void dummyAction();

// --- Menu Definition ---
// Declare all menu items
MenuItem mainMenu, profilesMenu, generatorMenu, crankMenu, ignitionMenu, injectorsMenu, canMenu, diagMenu, settingsMenu;
// Profiles
MenuItem profile1, profile2, profile3;
// Generator
MenuItem genType, genPwmDuty, genCanId;
MenuItem genTypePwm, genTypeOnOff, genTypeCan;
// Crank
MenuItem crankRpm, crankType, crankType60_2, crankType36_1;
// Ignition
MenuItem ignDwell, ignTiming;
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
  MenuItem crankMenu =     {"Crankshaft",    &mainMenu,    &crankRpm,       &ignitionMenu,  dummyAction};
  MenuItem ignitionMenu =  {"Ignition",      &mainMenu,    &ignDwell,       &injectorsMenu,  dummyAction};
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
// Ignition
MenuItem ignDwell =    {"- Dwell Time",  &ignitionMenu, nullptr,        &ignTiming,      actionEditDwellTime};
MenuItem ignTiming =   {"- Timing Adv",  &ignitionMenu, nullptr,        nullptr,         dummyAction};
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
void* valueToEdit = nullptr; // Use void pointer to handle different types
enum EditValueType { EDIT_INT, EDIT_UINT };
EditValueType currentEditType;
const char* editTitle = "";

void enterEditMode(int* value, const char* title) {
  isInEditMode = true;
  valueToEdit = value;
  currentEditType = EDIT_INT;
  editTitle = title;
}

void enterEditMode(unsigned int* value, const char* title) {
  isInEditMode = true;
  valueToEdit = value;
  currentEditType = EDIT_UINT;
  editTitle = title;
}

void exitEditMode() {
  isInEditMode = false;
  valueToEdit = nullptr;
}


void setupMenu() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Emulator v3.0");
  display.println("OLED Ready");
  display.display();
  delay(2000);
}

void loopMenu() {
  // The display logic will be more complex now
  updateDisplay();
}

// --- Navigation and Display Logic ---

// Pointer to the first item visible on the screen (for scrolling)
MenuItem* topVisibleItem = &mainMenu;

void drawStatusBar() {
  display.drawFastHLine(0, 7, display.width(), SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  // Time from RTC
  // DateTime now = rtc.now();
  // display.print(now.hour());
  // display.print(":");
  // display.print(now.minute());

  // Wi-Fi Status Icon
  display.setCursor(110, 0);
  if (WiFi.status() == WL_CONNECTED) {
    display.print("WiFi"); // Placeholder for a real icon
  } else {
    display.print("----");
  }
}

void updateDisplay() {
  display.clearDisplay();
  drawStatusBar();

  if (isInEditMode) {
    // Title
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(editTitle);
    display.drawFastHLine(0, 18, display.width(), SSD1306_WHITE);

    // Value
    display.setTextSize(3);
    display.setCursor(20, 30);
    if (currentEditType == EDIT_INT) {
      display.println(*(int*)valueToEdit);
    } else {
      display.println(*(unsigned int*)valueToEdit);
    }

    // Arrows hint
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("^");
    display.setCursor(0, 45);
    display.println("v");

  } else {
    display.setTextSize(1);
    MenuItem* item = topVisibleItem;
    int i = 0;
    // We can display up to 6 menu items on a 128x64 screen (with a status bar)
    while (item != nullptr && i < 6) {
      display.setCursor(0, 8 + (i * 9)); // Start below status bar, 9 pixels per line
      if (item == currentMenu) {
        // Invert text for selection
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        display.print(">");
        display.print(item->title);
        display.setTextColor(SSD1306_WHITE);
      } else {
        display.print(" ");
        display.print(item->title);
      }

      // Special handling to display current value next to the menu item
      if (item == &crankRpm) {
        display.setCursor(80, 8 + (i * 9));
        display.print(rpm);
      }
      if (item == &genPwmDuty) {
        display.setCursor(80, 8 + (i * 9));
        display.print(pwmDutyCycle);
        display.print("%");
      }

      item = item->nextSibling;
      i++;
    }
  }

  display.display();
}

void navigateNext() {
  if (currentMenu->nextSibling != nullptr) {
    // Find how many items are visible
    int visibleItems = 0;
    MenuItem* temp = topVisibleItem;
    while(temp != nullptr && visibleItems < 6) {
        if(temp == currentMenu) break;
        temp = temp->nextSibling;
        visibleItems++;
    }

    currentMenu = currentMenu->nextSibling;
    // If selection goes off screen, scroll
    if (visibleItems >= 5) {
      topVisibleItem = topVisibleItem->nextSibling;
    }
  }
}

void navigatePrev() {
    if (currentMenu->parent != nullptr && currentMenu != currentMenu->parent->firstChild) {
        MenuItem* prev = currentMenu->parent->firstChild;
        while (prev != nullptr && prev->nextSibling != currentMenu) {
            prev = prev->nextSibling;
        }
        currentMenu = prev;
        if (currentMenu == topVisibleItem->parent->firstChild) {
            topVisibleItem = currentMenu;
        }
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
    topVisibleItem = currentMenu->parent->firstChild; // Show parent menu from the top
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

void actionEditDwellTime() {
  enterEditMode(&dwellTime_ms, "Dwell (ms)");
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
