# AutoGarage v2 Architecture & Firmware Audit Report

## 1. Code Review & Runtime Inspection Findings

### Core & Runtime Data Pipeline
- `EngineState` and `SensorValue` structures exist in `SystemState.h`, but were not previously updated in a continuous runtime loop or dispatched through a unified Data Engine.
- `EventQueue` overflowed silently under high signal rates without incrementing overflow metrics or warning diagnostic systems.
- `updateEngineStateFromSources()` was a static declaration in `Globals.cpp` without active background execution or fault injection ground truth hooks.

### Hardware & Subsystems
- **GPIO Pins**: All GPIO conflicts (GPIO 6-11 SPI flash, I2C SDA collision, GPIO 33 injector/ADC overlap) have been resolved.
- **Signal Capture**: Interrupt handlers (`RISING` and `FALLING`) are attached for CKP and CMP capturing microsecond edge timestamps.
- **Crank Generator**: Uses non-blocking `esp_timer` high-resolution timer.
- **Storage & Web**: LittleFS filesystem assets reside in `data/` (`index.html`, `style.css`, `script.js`).

## 2. Identified Fixes
1. Implement runtime `DataEngine` tick loop updating `EngineState` and `SensorValue` based on `DataMode` (`SIMULATION`, `LOOPBACK`, `REAL`, `HYBRID`).
2. Implement overflow counter in `EventQueue` to track dropped events.
3. Update `.github/workflows/main.yml` branch pattern to `feature/autogarage-v2-data-engine-13158850612268727049`.
