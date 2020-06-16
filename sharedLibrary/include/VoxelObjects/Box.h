#pragma once
#include <glm/vec3.hpp>
struct Box
{
  glm::vec3 mPos;
  glm::vec3 mExtents;

  Box(const glm::vec3 &origin, const glm::vec3 &extents) : mPos(origin), mExtents(extents)
  {
  }
};