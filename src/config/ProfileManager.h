#ifndef PROFILE_MANAGER_H
#define PROFILE_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>
#include "core/SystemState.h"

#define PROFILE_CONFIG_VERSION 2
#define PROFILE_EEPROM_SIZE 512

struct VersionedProfile {
    uint8_t configVersion;
    uint32_t targetRpm;
    uint8_t crankPattern;
    bool cmpEnabled;
    float cmpPhaseDegrees;
    uint32_t injectorPulseWidthUs;
    uint8_t injectorMode;
    uint32_t ignitionDwellUs;
    float ignitionAdvanceDeg;
    uint32_t canBitrate;
    uint32_t checksum;
};

class ProfileManager {
public:
    static bool begin();
    static bool saveProfile(uint8_t slot = 0);
    static bool loadProfile(uint8_t slot = 0);
    static uint32_t calculateCrc(const VersionedProfile& profile);
};

#endif // PROFILE_MANAGER_H
