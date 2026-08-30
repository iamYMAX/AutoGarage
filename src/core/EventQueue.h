#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"

#ifndef EVENT_QUEUE_SIZE
#define EVENT_QUEUE_SIZE 256
#endif

class EventQueue {
private:
    EngineEvent buffer[EVENT_QUEUE_SIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
    portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;

public:
    EventQueue() : head(0), tail(0) {}

    bool push(const EngineEvent& event) {
        portENTER_CRITICAL_ISR(&spinlock);
        uint32_t nextHead = (head + 1) % EVENT_QUEUE_SIZE;
        if (nextHead == tail) {
            portEXIT_CRITICAL_ISR(&spinlock);
            return false;
        }
        buffer[head] = event;
        head = nextHead;
        portEXIT_CRITICAL_ISR(&spinlock);
        return true;
    }

    bool pop(EngineEvent& event) {
        portENTER_CRITICAL(&spinlock);
        if (head == tail) {
            portEXIT_CRITICAL(&spinlock);
            return false;
        }
        event = buffer[tail];
        tail = (tail + 1) % EVENT_QUEUE_SIZE;
        portEXIT_CRITICAL(&spinlock);
        return true;
    }

    bool isEmpty() const {
        return head == tail;
    }

    uint32_t size() const {
        if (head >= tail) return head - tail;
        return EVENT_QUEUE_SIZE - (tail - head);
    }
};

extern EventQueue globalEventQueue;

#endif // EVENT_QUEUE_H
