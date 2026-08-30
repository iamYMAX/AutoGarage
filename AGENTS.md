# AutoGarage Agent Guidelines

AutoGarage v2 Agent Infrastructure & Development Rules.

## Agent Roles
- **Architect**: Task definition, YAML contracts, acceptance criteria.
- **Coder**: Firmware implementation, data engines, bug fixes.
- **Tester**: PlatformIO compilation (`pio run`), LittleFS build (`pio run -t buildfs`), CI.
- **Reviewer**: Quality gate, realtime ISR safety, memory, concurrency review.

## ESP32 Constraints
- No GPIO 6-11 (SPI flash).
- ADC1 channels only for Wi-Fi compatibility.
- Microsecond timing via `getMonotonicTimestampUs()`.
