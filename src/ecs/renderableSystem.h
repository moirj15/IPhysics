#pragma once

#include "system.h"

namespace ecs
{
class World;
class RenerableSystem final : public System
{
  World *m_world;

public:
  explicit RenerableSystem(World *world);
  ~RenerableSystem() = default;
  void Update(f32 t) override;
};

} // namespace ecs
