# AutoGarage v2 Pinout Specification

| Function | GPIO | Direction | Notes |
| --- | --- | --- | --- |
| CKP OUT | 27 | OUT | Timer ISR |
| CMP OUT | 14 | OUT | Timer ISR |
| CKP IN | 34 | IN | Capture |
| CMP IN | 35 | IN | Capture |
| Injector 1-6 | 4,16,17,25,26,33 | OUT | Driver required |
| Ignition Coil | 32 | OUT | Driver required |
| CAN CS/INT/SPI | 5, 15, 18, 19, 23 | I/O | VSPI |
| SD CS | 13 | OUT | SPI |
| ADC Bat/Cur | 36, 39 | IN | ADC1 |
| OLED/RTC I2C | 21, 22 | I/O | I2C |
