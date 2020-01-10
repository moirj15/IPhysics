#pragma once

#include "../common.h"
#include "componentPool.h"
#include "entity.h"

#include <unordered_map>
#include <vector>

namespace ecs
{

class System;

class World
{
  ComponentPool m_components;
  std::vector<System *> m_systems;
  std::unordered_map<EntityID, Entity> m_entities;
  std::vector<std::array<EntityID, ComponentPool::POOL_SIZE>> m_freeIDs;
  u64 m_nextFreeIDIndex;

public:
  World();
  ~World();

  EntityID CreateEntity();

  template<typename T>
  void AddComponent(const EntityID id, T data)
  {
    auto *component = m_components.GetComponent<T>(id);
    *component = data;
    auto *entity = m_entities[id];
    entity->m_components.insert({typeid(component), data});
  }

  void DestroyEntity(const EntityID id);
  Entity *GetEntity(const EntityID id);
  bool IsIDValid(const EntityID id);

private:
  EntityID GetNextFreeID();
  void MarkIDAsFree(const EntityID id);
};
} // namespace ecs
