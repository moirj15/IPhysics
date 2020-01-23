#include "ecs.h"

namespace ecs
{

World::World() : mComponents(1), mFreeIDs(1)
{
}

World::~World()
{
}

EntityID World::CreateEntity()
{
  auto id = GetNextFreeID();
  // mEntities.insert({id, Entity(id)});
  return id;
}

std::vector<Entity *> World::GetEntitiesOfType(const u64 type)
{
  std::vector<Entity *> ret;
  for (const auto &entityPair : mEntities)
  {
    auto *entity = entityPair.second;
    if (entity->mID & type)
    {
      ret.emplace_back(entity);
    }
  }
  return ret;
}

void World::DestroyEntity(const EntityID id)
{
  mEntities.erase(id);
  MarkIDAsFree(id);
}

Entity *World::GetEntity(const EntityID id)
{
  return &mEntities[id];
}

bool World::IsIDValid(const EntityID id)
{
  // TODO:
  return false;
}

// Private
EntityID World::GetNextFreeID()
{
  // TODO: this is a lazy and inefficient way to do this, make it more efficient when it becomes
  // a problem
  for (auto &page : mFreeIDs)
  {
    for (u64 i = 0; i < page.size(); i++)
    {
      if (page[i] & FREE_MASK)
      {
        page[i] = page[i] & (~FREE_MASK);
        return page[i];
      }
    }
  }
}

void World::MarkIDAsFree(const EntityID id)
{
  // TODO: this is a lazy and inefficient way to do this, make it more efficient when it becomes
  // a problem
  for (u64 pageIndex = 0; pageIndex < mFreeIDs.size(); pageIndex++)
  {
    for (u64 i = 0; i < mFreeIDs[pageIndex].size(); i++)
    {
      if ((i * pageIndex) == (id & 0xffff))
      {
        mFreeIDs[pageIndex][i] = FREE_MASK;
      }
    }
  }
}

} // namespace ecs
