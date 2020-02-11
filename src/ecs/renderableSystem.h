#pragma once

#include "component.h"
#include "system.h"

class RendererBackend;
struct Window;

namespace ecs
{
class World;
class RenerableSystem final : public System
{
  World *mWorld;
  RendererBackend *mRenderer;

  static constexpr u64 RENDERABLE_TUPLE_BIT_SET = RENDERABLE_BIT | TRANSFORM_BIT | PHYSICS_BIT;

public:
  explicit RenerableSystem(World *world, Window *window);
  ~RenerableSystem() = default;
  void Update(f32 t) override;
};

} // namespace ecs
