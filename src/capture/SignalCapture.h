#ifndef SIGNAL_CAPTURE_H
#define SIGNAL_CAPTURE_H

#include <Arduino.h>
#include "config/PinConfig.h"
#include "core/Event.h"
#include "core/EventQueue.h"
#include "core/Timestamp.h"
#include "core/SystemState.h"

class SignalCapture {
private:
    bool enabled;

    static void IRAM_ATTR onCkpRisingIsr();
    static void IRAM_ATTR onCkpFallingIsr();
    static void IRAM_ATTR onCmpRisingIsr();
    static void IRAM_ATTR onCmpFallingIsr();

public:
    SignalCapture();
    ~SignalCapture();

    bool begin();
    void stop();
    bool isEnabled() const { return enabled; }
};

extern SignalCapture gSignalCapture;

#endif // SIGNAL_CAPTURE_H
