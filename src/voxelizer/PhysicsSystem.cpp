#include "PhysicsSystem.hpp"

PhysicsSystem::PhysicsSystem(ResourceSystem *resource_system) : System(resource_system)
{}

void PhysicsSystem::Step(f32 delta, const std::vector<Entity> &entities)
{
}
void PhysicsSystem::MeshLoaded(const Handle<objs::Mesh> &handle)
{
}
