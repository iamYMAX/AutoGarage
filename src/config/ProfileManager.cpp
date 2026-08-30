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
    prof.globalDataMode = (uint8_t)gSystemState.globalDataMode;
    prof.sourceCkp = (uint8_t)gSystemState.sourceCkp;
    prof.sourceCmp = (uint8_t)gSystemState.sourceCmp;
    prof.sourceTps = (uint8_t)gSystemState.sourceTps;
    prof.sourceMap = (uint8_t)gSystemState.sourceMap;
    prof.sourceEct = (uint8_t)gSystemState.sourceEct;
    prof.sourceIat = (uint8_t)gSystemState.sourceIat;
    prof.sourceBattery = (uint8_t)gSystemState.sourceBattery;
    prof.sourceCan = (uint8_t)gSystemState.sourceCan;
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

    gSystemState.globalDataMode = (DataMode)prof.globalDataMode;
    gSystemState.sourceCkp = (SensorSourceType)prof.sourceCkp;
    gSystemState.sourceCmp = (SensorSourceType)prof.sourceCmp;
    gSystemState.sourceTps = (SensorSourceType)prof.sourceTps;
    gSystemState.sourceMap = (SensorSourceType)prof.sourceMap;
    gSystemState.sourceEct = (SensorSourceType)prof.sourceEct;
    gSystemState.sourceIat = (SensorSourceType)prof.sourceIat;
    gSystemState.sourceBattery = (SensorSourceType)prof.sourceBattery;
    gSystemState.sourceCan = (SensorSourceType)prof.sourceCan;
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
