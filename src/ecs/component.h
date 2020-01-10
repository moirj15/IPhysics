#pragma once

#include "../common.h"

#include <string>

namespace ecs
{
struct Component
{
};

struct Renderable
{
  std::string m_meshName;
  u64 m_meshHandle;
  std::string m_shaderName;
  u64 m_shaderHandle;
  std::string m_textureName;
  u64 m_textureHandle;
};

struct Physics
{
};
} // namespace ecs
