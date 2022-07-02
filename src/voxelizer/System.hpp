#pragma once

#include <vector>

class System {
protected:
    enum class EventType {
        MoveForward,
        MoveBackwards,
        MoveLeft,
        MoveRight,
        LeftClick,
        RightClick,
        CtrlLeftClick
    };
    std::vector<EventType> m_event_queue;
public:
    virtual ~System() = default;
    void EnqueueEvents(const std::vector<EventType> &event_queue) {
        m_event_queue = event_queue;
    }
    virtual void Step() = 0;
};