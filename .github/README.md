# AutoGarage Infrastructure & Agent Development Loop

## Development Cycle

```text
1. Architect creates task contract (.github/tasks/*.yml)
2. Coder implements task requirements
3. Tester runs build verification (pio run, pio run -t buildfs)
4. Reviewer evaluates changes and updates review contract (.github/reviews/*.yml)
```
