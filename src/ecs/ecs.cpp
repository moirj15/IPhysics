#include "ecs.h"

namespace ecs
{

World::World() : m_components(1), m_freeIDs(1)
{
}

World::~World()
{
}

EntityID World::CreateEntity()
{
  auto id = GetNextFreeID();
  // m_entities.insert({id, Entity(id)});
  return id;
}

void World::DestroyEntity(const EntityID id)
{
  m_entities.erase(id);
  MarkIDAsFree(id);
}

Entity *World::GetEntity(const EntityID id)
{
  return &m_entities[id];
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
  for (auto &page : m_freeIDs)
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
  for (u64 pageIndex = 0; pageIndex < m_freeIDs.size(); pageIndex++)
  {
    for (u64 i = 0; i < m_freeIDs[pageIndex].size(); i++)
    {
      if ((i * pageIndex) == (id & 0xffff))
      {
        m_freeIDs[pageIndex][i] = FREE_MASK;
      }
    }
  }
}

} // namespace ecs
