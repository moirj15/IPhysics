#pragma once

#include "../common.h"

#include <typeindex>
#include <unordered_map>

namespace ecs
{
using EntityID = u64;

struct Component;

constexpr u64 FREE_MASK = 0x8000000000000000;
struct Entity
{
  EntityID mID;
  std::unordered_map<std::type_index, Component *> mComponents;

  Entity() = default;
  Entity(const EntityID id) : mID(id)
  {
  }

  template<typename T>
  constexpr T *Sibling()
  {
    return static_cast<T *>(mComponents[typeid(T *)]);
  }
};

} // namespace ecs
