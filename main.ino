// Main file for the engine signal emulator project.
// This file will contain the main setup and loop functions.

// Global variables
int rpm = 1000; // Revolutions per minute

// Global variables
int rpm = 1000; // Revolutions per minute

// Global variables
int rpm = 1000; // Revolutions per minute
int pwmDutyCycle = 50; // PWM duty cycle in %

// Global variables
int rpm = 1000; // Revolutions per minute
int pwmDutyCycle = 50; // PWM duty cycle in %

// Global variables
int rpm = 1000;
int pwmDutyCycle = 50;
int totalTeeth = 60;
int missingTeeth = 2;
int injectionMode = 0;
unsigned int pulseWidth = 5;

// Global variables
int rpm = 1000;
int pwmDutyCycle = 50;
int totalTeeth = 60;
int missingTeeth = 2;
int injectionMode = 0;
unsigned int pulseWidth = 5;
unsigned int dwellTime_ms = 3; // For menu editing

void setup() {
  Serial.begin(115200);

  // Initialize networking first
  setupWiFi();
  setupWebServer();

  // Initialize other modules
  setupProfiles(); // Load settings first
  setupCrank();
  setupInjectors();
  setupGenerator();
  setupIgnition(); // Add ignition setup
  setupMenu();
  setupEncoder();
  setupCan();
  setupLogger();
  setupRtc();
  setupDiagnostics();
}

void loop() {
  // Run all module loops
  loopWiFi();
  loopWebServer();
  loopEncoder(); // Read encoder first
  loopCrank();
  loopGenerator();
  loopIgnition(); // Handle ignition timing
  loopMenu();
  loopCan();
  loopLogger();
  loopRtc();
  loopDiagnostics();
  // loopInjectors(); // loopInjectors is currently empty
}
