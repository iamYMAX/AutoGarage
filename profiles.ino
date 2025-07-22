// Module for saving and loading configuration profiles.

#include <EEPROM.h>

// Define a structure to hold all settings
struct Profile {
  int rpm;
  int totalTeeth;
  int missingTeeth;
  int pwmDutyCycle;
  int injectionMode;
  unsigned int pulseWidth;
  GeneratorType generatorType;
  unsigned int dwellTime_ms; // Added ignition dwell time
  int checksum;
};

void saveProfile(int profileNumber) {
  Profile currentProfile;
  currentProfile.rpm = rpm;
  currentProfile.totalTeeth = totalTeeth;
  currentProfile.missingTeeth = missingTeeth;
  currentProfile.pwmDutyCycle = pwmDutyCycle;
  currentProfile.injectionMode = injectionMode;
  currentProfile.pulseWidth = pulseWidth;
  currentProfile.generatorType = currentGeneratorType;
  currentProfile.dwellTime_ms = dwellTime_ms;

  // Calculate a simple checksum
  currentProfile.checksum = currentProfile.rpm + currentProfile.pwmDutyCycle + (int)currentProfile.generatorType + currentProfile.dwellTime_ms;

  int address = profileNumber * sizeof(Profile);
  EEPROM.put(address, currentProfile);
}

bool loadProfile(int profileNumber) {
  Profile loadedProfile;
  int address = profileNumber * sizeof(Profile);
  EEPROM.get(address, loadedProfile);

  // Verify checksum
  if (loadedProfile.checksum == (loadedProfile.rpm + loadedProfile.pwmDutyCycle + (int)loadedProfile.generatorType + loadedProfile.dwellTime_ms)) {
    rpm = loadedProfile.rpm;
    totalTeeth = loadedProfile.totalTeeth;
    missingTeeth = loadedProfile.missingTeeth;
    pwmDutyCycle = loadedProfile.pwmDutyCycle;
    injectionMode = loadedProfile.injectionMode;
    pulseWidth = loadedProfile.pulseWidth;
    currentGeneratorType = loadedProfile.generatorType;
    dwellTime_ms = loadedProfile.dwellTime_ms;

    // Update modules with new values
    setRpm(rpm);
    setGeneratorDutyCycle(pwmDutyCycle);
    setGeneratorType(currentGeneratorType);
    setDwellTime(dwellTime_ms);
    return true; // Load successful
  }
  return false; // Load failed (checksum mismatch)
}

void setupProfiles() {
  // Try to load the default profile (e.g., profile 0) on startup
  // if (!loadProfile(0)) {
  //   // If it fails, maybe load default hardcoded values
  // }
}
