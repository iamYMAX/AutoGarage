# AutoGarage v2 System Architecture & Agent Workflow Specification

## Architecture Overview

AutoGarage v2 is a modular, high-precision engine data collection node and emulating core built for the ESP32.

```text
                        VEHICLE / TEST RIG
                                │
             ┌──────────────────┼──────────────────┐
             │                  │                  │
          CAN Bus          CKP/CMP Sensors    ADC Probes
             │                  │                  │
             └──────────────────┼──────────────────┘
                                │
                            ESP32 Node
                                │
        ┌───────────────────────┴───────────────────────┐
        │                 Data Engine                   │
        │  [SIMULATION | LOOPBACK | REAL | HYBRID]     │
        └───────────────────────┬───────────────────────┘
                                │
                        Microsecond Events
                       (EngineEvent Queue)
                                │
             ┌──────────────────┴──────────────────┐
             │                                     │
       Serial JSONL                             SD Card
             │                                 Fallback
             ▼                                     │
      Python Collector ───────────────► Engine Dataset
```

## Data Engine Modes

- **`SIMULATION`**: Modeled sensor signals generated internally.
- **`LOOPBACK`**: Internal generator signals routed to external capture pins for hardware timing verification.
- **`REAL`**: Physical sensor readers (ADC, GPIO interrupts, CAN transceivers).
- **`HYBRID`**: Individual source assignment for each of the 11 engine channels.

## Agent Infrastructure & CI Cycle

1. **Architect**: Defines tasks and acceptance criteria in Task Contracts.
2. **Coder**: Implements C++ firmware and python collector tools.
3. **Tester**: Validates builds (`pio run`, `pio run -t buildfs`).
4. **Reviewer**: Reviews code quality, memory usage, and hardware constraints.
