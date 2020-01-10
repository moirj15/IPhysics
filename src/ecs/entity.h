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
  EntityID m_id;
  std::unordered_map<std::type_index, Component *> m_components;

  Entity() = default;
  Entity(const EntityID id) : m_id(id)
  {
  }

  template<typename T>
  constexpr T *Sibling()
  {
    return static_cast<T *>(m_components[typeid(T *)]);
  }
};

} // namespace ecs
