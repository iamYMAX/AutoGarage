// Module for handling CAN bus communication.

#include <SPI.h>
#include <mcp2515.h>

#define CAN_CS_PIN 9
#define CAN_INT_PIN 8

MCP2515 mcp2515(CAN_CS_PIN);
struct can_frame canMsg;

void setupCan() {
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  // Prepare a standard CAN message
  canMsg.can_id  = 0x123; // Example CAN ID
  canMsg.can_dlc = 8;     // Data length
}

void loopCan() {
  // Send CAN message periodically
  static unsigned long lastCanSendTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastCanSendTime > 100) { // Send every 100ms
    lastCanSendTime = currentTime;
    sendRpmCanMessage();
  }
}

void sendRpmCanMessage() {
  // Encode RPM into two bytes (as an example)
  byte rpmData[8];
  rpmData[0] = (rpm >> 8) & 0xFF; // High byte
  rpmData[1] = rpm & 0xFF;       // Low byte
  for (int i = 2; i < 8; i++) {
    rpmData[i] = 0x00;
  }

  sendCanMessage(0x123, 8, rpmData);
}

void sendCanMessage(unsigned long id, byte dlc, byte *data) {
  struct can_frame newCanMsg;
  newCanMsg.can_id  = id;
  newCanMsg.can_dlc = dlc;
  for (int i = 0; i < dlc; i++) {
    newCanMsg.data[i] = data[i];
  }

  mcp2515.sendMessage(&newCanMsg);
}
