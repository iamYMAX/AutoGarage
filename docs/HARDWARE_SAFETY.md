# AutoGarage v2 Hardware Safety Guidelines

ESP32 GPIO operates at 3.3V logic. Direct connection to 12V vehicle circuits will cause hardware destruction.
- Voltage dividers on ADC inputs.
- MOSFET/low-side drivers on injectors.
- High-side driver on ignition coil.
- Optocouplers/level shifters on CKP/CMP inputs.
