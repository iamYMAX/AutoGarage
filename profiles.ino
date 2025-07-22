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
  // Add other settings as needed
  int checksum; // To verify data integrity
};

void saveProfile(int profileNumber) {
  Profile currentProfile;
  currentProfile.rpm = rpm;
  // currentProfile.totalTeeth = totalTeeth; // Assuming these are global
  // currentProfile.missingTeeth = missingTeeth;
  currentProfile.pwmDutyCycle = pwmDutyCycle;
  // currentProfile.injectionMode = injectionMode;
  // currentProfile.pulseWidth = pulseWidth;

  // Calculate a simple checksum
  currentProfile.checksum = currentProfile.rpm + currentProfile.pwmDutyCycle;

  int address = profileNumber * sizeof(Profile);
  EEPROM.put(address, currentProfile);
}

bool loadProfile(int profileNumber) {
  Profile loadedProfile;
  int address = profileNumber * sizeof(Profile);
  EEPROM.get(address, loadedProfile);

  // Verify checksum
  if (loadedProfile.checksum == (loadedProfile.rpm + loadedProfile.pwmDutyCycle)) {
    rpm = loadedProfile.rpm;
    pwmDutyCycle = loadedProfile.pwmDutyCycle;
    // Update other variables...

    // Update modules with new values
    setRpm(rpm);
    setGeneratorDutyCycle(pwmDutyCycle);
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
