#pragma once

#include <vector>
#include "Common.h"

class System
{
public:
    enum class Event {
        MoveForward,
        MoveBackwards,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        MoveBoost,
        LeftClick,
        RightClick,
        CtrlLeftClick,
    };

protected:
    std::vector<Event> m_event_queue;

public:
    virtual ~System() = default;
    void EnqueueEvents(const std::vector<Event> &event_queue) { m_event_queue = event_queue; }
    virtual void Step(f32 delta) = 0;
};
