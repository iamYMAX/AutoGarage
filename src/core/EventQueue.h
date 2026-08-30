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

public:
    EventQueue() : head(0), tail(0) {}

    bool push(const EngineEvent& event) {
        uint32_t nextHead = (head + 1) % EVENT_QUEUE_SIZE;
        if (nextHead == tail) {
            return false;
        }
        buffer[head] = event;
        head = nextHead;
        return true;
    }

    bool pop(EngineEvent& event) {
        if (head == tail) {
            return false;
        }
        event = buffer[tail];
        tail = (tail + 1) % EVENT_QUEUE_SIZE;
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
