#pragma once

#include "Common.h"

#include <vector>
#include "Handle.hpp"

namespace objs
{
struct Mesh;
}

class ResourceSystem;
struct Entity;

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
    ResourceSystem *m_resources_system;

public:
    explicit System(ResourceSystem *resources_system) : m_resources_system(resources_system) {}
    virtual ~System() = default;
    void EnqueueEvents(const std::vector<Event> &event_queue) { m_event_queue = event_queue; }
    virtual void Step(f32 delta, const std::vector<Entity> &entities) = 0;
    virtual void MeshLoaded(const Handle<objs::Mesh> &handle) = 0;
};
