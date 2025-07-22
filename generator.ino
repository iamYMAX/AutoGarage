// Module for generating the generator regulator signal.

// Pin definitions for ESP32
// ESP32 uses ledc for PWM, which requires channel assignment.
// This is a conceptual change for now.
#define PWM_RELAY_PIN 25 // A common GPIO pin for PWM on ESP32
#define ON_OFF_RELAY_PIN 26
#define ANALOG_FEEDBACK_PIN 34 // ADC1_CH6, suitable for ADC on ESP32

// Enum for generator control types
enum GeneratorType {
  GEN_TYPE_PWM,
  GEN_TYPE_ON_OFF,
  GEN_TYPE_CAN
};

// Global generator settings
GeneratorType currentGeneratorType = GEN_TYPE_PWM; // Default type
extern int pwmDutyCycle; // Defined in main.ino
int onOffState = LOW; // For ON/OFF control

// --- Function Prototypes for different modes ---
void controlPwmRelay();
void controlOnOffRelay();
void controlCanRelay();

void setupGenerator() {
  pinMode(PWM_RELAY_PIN, OUTPUT);
  pinMode(ON_OFF_RELAY_PIN, OUTPUT);
  pinMode(ANALOG_FEEDBACK_PIN, INPUT);

  // Set initial state
  digitalWrite(ON_OFF_RELAY_PIN, onOffState);
  // Set initial PWM
  setGeneratorDutyCycle(pwmDutyCycle);
}

void loopGenerator() {
  // Call the appropriate control function based on the selected mode
  switch (currentGeneratorType) {
    case GEN_TYPE_PWM:
      controlPwmRelay();
      break;
    case GEN_TYPE_ON_OFF:
      controlOnOffRelay();
      break;
    case GEN_TYPE_CAN:
      controlCanRelay();
      break;
  }
}

// Function to change the generator type from the menu
void setGeneratorType(GeneratorType newType) {
  if (currentGeneratorType != newType) {
    currentGeneratorType = newType;
    // Potentially disable outputs of other types when switching
    notifyClients();
  }
}

// --- PWM Control ---
void setGeneratorDutyCycle(int dutyCycle) {
  if (pwmDutyCycle != dutyCycle && dutyCycle >= 0 && dutyCycle <= 100) {
    pwmDutyCycle = dutyCycle;
    int pwmValue = map(pwmDutyCycle, 0, 100, 0, 255);
    analogWrite(PWM_RELAY_PIN, pwmValue);
    notifyClients();
  }
}

void controlPwmRelay() {
  // PWM is handled by analogWrite, so this can be empty for now
}

// --- ON/OFF Control ---
void setOnOffState(bool newState) {
  onOffState = newState ? HIGH : LOW;
}

void controlOnOffRelay() {
  digitalWrite(ON_OFF_RELAY_PIN, onOffState);
}

// --- CAN Control ---
void controlCanRelay() {
  // Send CAN message for generator control periodically
  static unsigned long lastGenCanSendTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastGenCanSendTime > 200) { // Send every 200ms
    lastGenCanSendTime = currentTime;

    // Example CAN message for a generator
    // This would be customized based on the specific vehicle protocol
    unsigned long canId = 0x1A6;
    byte canData[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    sendCanMessage(canId, 8, canData); // Assumes a generic CAN sending function
  }
}
