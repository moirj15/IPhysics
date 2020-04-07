#pragma once

#include "../common.h"

#include <LinearMath/btVector3.h>
#include <glm/vec3.hpp>

NODISCARD inline glm::vec3 ToGLM(const btVector3 &bv)
{
  return glm::vec3(bv.x(), bv.y(), bv.z());
}

NODISCARD inline btVector3 ToBullet(const glm::vec3 &v)
{
  return btVector3(v.x, v.y, v.z);
}