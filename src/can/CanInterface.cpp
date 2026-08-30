#include "CanInterface.h"

CanInterface gCanInterface;

CanInterface::CanInterface() : mcp2515(PIN_CAN_CS), initialized(false), currentBitrate(500000) {}

CanInterface::~CanInterface() {}

bool CanInterface::begin(uint32_t bitrate) {
    currentBitrate = bitrate;
    gSystemState.canBitrate = bitrate;

    SPI.begin(PIN_CAN_SCK, PIN_CAN_MISO, PIN_CAN_MOSI, PIN_CAN_CS);
    mcp2515.reset();

    CAN_SPEED speedEnum = CAN_500KBPS;
    if (bitrate == 250000) {
        speedEnum = CAN_250KBPS;
    } else if (bitrate == 125000) {
        speedEnum = CAN_125KBPS;
    }

    if (mcp2515.setBitrate(speedEnum, MCP_8MHZ) != MCP2515::ERROR_OK) {
        initialized = false;
        return false;
    }

    if (mcp2515.setNormalMode() != MCP2515::ERROR_OK) {
        initialized = false;
        return false;
    }

    pinMode(PIN_CAN_INT, INPUT);
    initialized = true;
    return true;
}

bool CanInterface::sendFrame(uint32_t id, bool ext, uint8_t dlc, const uint8_t* data) {
    if (!initialized) return false;

    struct can_frame frame;
    frame.can_id = id;
    if (ext) frame.can_id |= CAN_EFF_FLAG;
    frame.can_dlc = dlc;
    for (uint8_t i = 0; i < dlc && i < 8; i++) {
        frame.data[i] = data[i];
    }

    return (mcp2515.sendMessage(&frame) == MCP2515::ERROR_OK);
}

void CanInterface::poll() {
    if (!initialized) return;

    struct can_frame frame;
    while (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
        uint64_t ts = getMonotonicTimestampUs();

        EngineEvent ev;
        ev.timestamp_us = ts;
        ev.event_type = EVENT_CAN_FRAME;
        ev.channel = 0;
        ev.reserved = frame.can_dlc;
        ev.value = (int32_t)(frame.can_id & 0x1FFFFFFF);

        globalEventQueue.push(ev);
        gSystemState.canFramesCount++;
    }
}
