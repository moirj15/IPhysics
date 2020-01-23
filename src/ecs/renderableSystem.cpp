#include "renderableSystem.h"

#include "../renderer/renderer.h"
#include "ecs.h"
namespace ecs
{
RenerableSystem::RenerableSystem(World *world, Window *window) :
    mWorld(world), mRenderer(new Renderer(window))
{
}

void RenerableSystem::Update(f32 t)
{
  auto entities = mWorld->GetEntitiesOfType(RENDERABLE_TUPLE_BIT_SET);
  for (auto *entity : entities)
  {
    auto *renderable = entity->Sibling<Renderable>();
    auto *transform = entity->Sibling<Transform>();
    auto *physics = entity->Sibling<Physics>();

    if (physics->mCollision)
    {
      // TODO: update mesh
    }
    else
    {
      Command command(CommandType::DrawSolid);
      command.mMeshHandle = renderable->mMeshHandle;
      command.mColor = {1.0f, 1.0f, 0.0f};
    }
  }
}
} // namespace ecs
