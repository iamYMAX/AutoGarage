# AutoGarage v2 System Architecture

AutoGarage v2 data engine node architecture on ESP32.
- Microsecond event pipeline (`EngineEvent`).
- Hardware timer ISR CKP/CMP generation (`CrankGenerator`).
- Interrupt capture (`SignalCapture`).
- CAN bus logger (`CanInterface`).
- ADC manager, Injector, Ignition, Logger.
