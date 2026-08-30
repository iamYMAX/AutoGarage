# AutoGarage Agent Workflow & Development Cycle

```text
1. Architect/ChatGPT creates task contract in .github/tasks/current.yml
2. Task is saved and committed
3. Jules reads task contract and implements requirements
4. Jules creates commit
5. GitHub Actions executes build and filesystem tests
6. Reviewer evaluates changes and updates .github/reviews/current.yml
7. review-gate checks status (passed vs failed/pending)
8. Upon FAIL, Jules receives feedback list and repeats cycle
```
