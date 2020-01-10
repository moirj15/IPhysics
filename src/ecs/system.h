#pragma once

#include "../common.h"

namespace ecs
{
class System
{
public:
  virtual ~System() = default;
  virtual void Update(f32 t) = 0;
};
} // namespace ecs
