#pragma once

#include "System.hpp"

class PhysicsSystem : public System
{
public:
    void Step(f32 delta) override;
};
