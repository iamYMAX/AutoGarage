#ifndef EVENT_H
#define EVENT_H

#include <Arduino.h>

enum EventType : uint8_t {
    EVENT_NONE = 0,
    EVENT_CKP_RISING = 1,
    EVENT_CKP_FALLING = 2,
    EVENT_CMP_RISING = 3,
    EVENT_CMP_FALLING = 4,
    EVENT_CAN_FRAME = 5,
    EVENT_INJECTOR_ON = 6,
    EVENT_INJECTOR_OFF = 7,
    EVENT_IGNITION_DWELL_START = 8,
    EVENT_IGNITION_FIRE = 9,
    EVENT_ADC_SAMPLE = 10,
    EVENT_SYSTEM_STATUS = 11
};

struct EngineEvent {
    uint64_t timestamp_us;
    uint8_t event_type;
    uint8_t channel;
    uint16_t reserved;
    int32_t value;
};

inline const char* getEventTypeName(uint8_t type) {
    switch (type) {
        case EVENT_CKP_RISING:           return "CKP_RISING";
        case EVENT_CKP_FALLING:          return "CKP_FALLING";
        case EVENT_CMP_RISING:           return "CMP_RISING";
        case EVENT_CMP_FALLING:          return "CMP_FALLING";
        case EVENT_CAN_FRAME:            return "CAN_FRAME";
        case EVENT_INJECTOR_ON:          return "INJECTOR_ON";
        case EVENT_INJECTOR_OFF:         return "INJECTOR_OFF";
        case EVENT_IGNITION_DWELL_START: return "IGN_DWELL_START";
        case EVENT_IGNITION_FIRE:        return "IGN_FIRE";
        case EVENT_ADC_SAMPLE:           return "ADC_SAMPLE";
        case EVENT_SYSTEM_STATUS:        return "SYS_STATUS";
        default:                         return "UNKNOWN";
    }
}

#endif // EVENT_H
