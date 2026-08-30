#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <Arduino.h>
#include <esp_timer.h>

inline uint64_t getMonotonicTimestampUs() {
    return (uint64_t)esp_timer_get_time();
}

#endif // TIMESTAMP_H
