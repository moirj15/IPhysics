#include "renderableSystem.h"

#include "../renderer/renderer.h"
#include "component.h"
#include "ecs.h"
namespace ecs
{
RenerableSystem::RenerableSystem(World *world, Window *window) :
    mWorld(world), mRenderer(new Renderer(window))
{
}

void RenerableSystem::Update(f32 t)
{
}
} // namespace ecs
