#pragma once

#include "system.h"

class Renderer;
struct Window;

namespace ecs
{
class World;
class RenerableSystem final : public System
{
  World *mWorld;
  Renderer *mRenderer;

public:
  explicit RenerableSystem(World *world, Window *window);
  ~RenerableSystem() = default;
  void Update(f32 t) override;
};

} // namespace ecs
