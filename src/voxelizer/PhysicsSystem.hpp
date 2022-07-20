#pragma once

#include "System.hpp"

class PhysicsSystem : public System
{
public:
    explicit PhysicsSystem(ResourceSystem *resource_system);
    void Step(f32 delta, const std::vector<Entity> &entities) override;
    void MeshLoaded(const Handle<objs::Mesh> &handle) override;
};
