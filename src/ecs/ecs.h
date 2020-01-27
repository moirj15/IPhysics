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
  ComponentPool mComponents;
  std::vector<System *> mSystems;
  std::unordered_map<EntityID, Entity> mEntities;
  std::vector<std::array<EntityID, ComponentPool::POOL_SIZE>> mFreeIDs;
  u64 mNextFreeIDIndex;

public:
  World();
  ~World();

  EntityID CreateEntity();

  template<typename T>
  void AddComponent(const EntityID id, T data)
  {
    auto *component = mComponents.GetComponent<T>(id);
    *component = data;
    // TODO: fix this if I end up using the ecs
    //    auto *entity = mEntities[id];
    //    entity->m_components.insert({typeid(component), data});
  }

  inline std::vector<Entity *> GetEntitiesOfType(const u64 type);

  void DestroyEntity(const EntityID id);
  Entity *GetEntity(const EntityID id);
  bool IsIDValid(const EntityID id);

private:
  EntityID GetNextFreeID();
  void MarkIDAsFree(const EntityID id);
};
} // namespace ecs
