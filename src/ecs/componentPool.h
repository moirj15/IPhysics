#pragma once
#include "../common.h"
#include "component.h"
#include "entity.h"

#include <array>
#include <vector>

namespace ecs
{

class ComponentPool
{
public:
  static constexpr u64 POOL_SIZE = 1024;

private:
  template<typename T>
  using ComponentArray = std::array<T, POOL_SIZE>;
  using ComponentTuple = std::tuple<ComponentArray<Renderable>, ComponentArray<Physics>>;

  std::vector<ComponentTuple> m_pages;
  static constexpr u64 PAGE_MASK = 0xf800;
  static constexpr u64 INDEX_MASK = 0x03ff;

public:
  ComponentPool(const u64 pageCount) : m_pages(pageCount > 0 ? pageCount : 1)
  {
  }

  template<typename T>
  inline Component *GetComponent(const EntityID id)
  {
    return &std::get<ComponentArray<T>>(GetPage(id))[id & INDEX_MASK];
  }

  template<typename T>
  inline void SetComponenet(const EntityID id, T t)
  {
    std::get<ComponentArray<T>>(GetPage(id))[id & INDEX_MASK] = t;
  }

  inline void AddPage()
  {
    m_pages.emplace_back();
  }

  inline u64 PageCount() const
  {
    return m_pages.size();
  }

private:
  inline ComponentTuple &GetPage(const EntityID id)
  {
    return m_pages[(id & PAGE_MASK) >> 11];
  }
};

} // namespace ecs
