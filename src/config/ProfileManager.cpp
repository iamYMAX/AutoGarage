#include "ProfileManager.h"

uint32_t ProfileManager::calculateCrc(const VersionedProfile& profile) {
    const uint8_t* p = (const uint8_t*)&profile;
    size_t len = sizeof(VersionedProfile) - sizeof(uint32_t);
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= p[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 1) crc = (crc >> 1) ^ 0xEDB88320;
            else crc >>= 1;
        }
    }
    return ~crc;
}

bool ProfileManager::begin() {
    return EEPROM.begin(PROFILE_EEPROM_SIZE);
}

bool ProfileManager::saveProfile(uint8_t slot) {
    VersionedProfile prof;
    prof.configVersion = PROFILE_CONFIG_VERSION;
    prof.targetRpm = gSystemState.targetRpm;
    prof.crankPattern = (uint8_t)gSystemState.crankPattern;
    prof.cmpEnabled = gSystemState.cmpEnabled;
    prof.cmpPhaseDegrees = gSystemState.cmpPhaseDegrees;
    prof.injectorPulseWidthUs = gSystemState.injectorPulseWidthUs;
    prof.injectorMode = gSystemState.injectorMode;
    prof.ignitionDwellUs = gSystemState.ignitionDwellUs;
    prof.ignitionAdvanceDeg = gSystemState.ignitionAdvanceDeg;
    prof.canBitrate = gSystemState.canBitrate;
    prof.checksum = calculateCrc(prof);

    EEPROM.put(slot * sizeof(VersionedProfile), prof);
    return EEPROM.commit();
}

bool ProfileManager::loadProfile(uint8_t slot) {
    VersionedProfile prof;
    EEPROM.get(slot * sizeof(VersionedProfile), prof);

    if (prof.configVersion != PROFILE_CONFIG_VERSION) {
        return false;
    }

    if (calculateCrc(prof) != prof.checksum) {
        return false;
    }

    gSystemState.targetRpm = prof.targetRpm;
    gSystemState.crankPattern = (CrankPattern)prof.crankPattern;
    gSystemState.cmpEnabled = prof.cmpEnabled;
    gSystemState.cmpPhaseDegrees = prof.cmpPhaseDegrees;
    gSystemState.injectorPulseWidthUs = prof.injectorPulseWidthUs;
    gSystemState.injectorMode = prof.injectorMode;
    gSystemState.ignitionDwellUs = prof.ignitionDwellUs;
    gSystemState.ignitionAdvanceDeg = prof.ignitionAdvanceDeg;
    gSystemState.canBitrate = prof.canBitrate;

    return true;
}
