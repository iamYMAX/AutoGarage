#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

#include <Arduino.h>

// Signal Generation
#define PIN_CKP_OUT         27
#define PIN_CMP_OUT         14

// Signal Capture
#define PIN_CKP_IN          34
#define PIN_CMP_IN          35

// Injectors
#define PIN_INJECTOR_1      4
#define PIN_INJECTOR_2      16
#define PIN_INJECTOR_3      17
#define PIN_INJECTOR_4      25
#define PIN_INJECTOR_5      26
#define PIN_INJECTOR_6      33

// Ignition
#define PIN_IGNITION_COIL    32

// CAN Bus (MCP2515 over VSPI)
#define PIN_CAN_CS          5
#define PIN_CAN_INT         15
#define PIN_CAN_SCK         18
#define PIN_CAN_MISO        19
#define PIN_CAN_MOSI        23

// SD Card
#define PIN_SD_CS           13

// Analog Inputs
#define PIN_ADC_BATTERY     36
#define PIN_ADC_CURRENT     39
#define PIN_ADC_GEN_FB      33  // Moved to dedicated ADC1 channel

// I2C
#define PIN_I2C_SDA         21
#define PIN_I2C_SCL         22

// Encoder
#define PIN_ENCODER_CLK     2
#define PIN_ENCODER_DT      0
#define PIN_ENCODER_BTN     12

#endif // PIN_CONFIG_H
