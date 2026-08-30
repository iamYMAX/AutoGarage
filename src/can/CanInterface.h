#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>
#include "config/PinConfig.h"
#include "core/Event.h"
#include "core/EventQueue.h"
#include "core/Timestamp.h"
#include "core/SystemState.h"

class CanInterface {
private:
    MCP2515 mcp2515;
    bool initialized;
    uint32_t currentBitrate;

public:
    CanInterface();
    ~CanInterface();

    bool begin(uint32_t bitrate = 500000);
    bool sendFrame(uint32_t id, bool ext, uint8_t dlc, const uint8_t* data);
    void poll();

    bool isInitialized() const { return initialized; }
};

extern CanInterface gCanInterface;

#endif // CAN_INTERFACE_H
