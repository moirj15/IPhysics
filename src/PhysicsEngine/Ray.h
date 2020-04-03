#pragma once

#include <LinearMath/btVector3.h>
#include <glm/vec3.hpp>

struct Ray
{
  btVector3 mOrigin;
  btVector3 mDirection;

  Ray(const glm::vec3 &start, const glm::vec3 &end) :
      mOrigin(start.x, start.y, start.z), mDirection(btVector3(end.x, end.y, end.z) - mOrigin)
  {
    mOrigin.normalize();
    mDirection.normalize();
  }
};