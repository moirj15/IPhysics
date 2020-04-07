#pragma once

#include <LinearMath/btVector3.h>
#include <glm/vec3.hpp>

struct Ray
{
  //   btVector3 mOrigin;
  //   btVector3 mDirection;
  glm::vec3 mOrigin;
  glm::vec3 mDirection;

  Ray(const glm::vec3 &start, const glm::vec3 &end) :
      mOrigin(start), mDirection((end - mOrigin) * 1000.0f)
  {
    //     mOrigin.normalize();
    //     mDirection.normalize();
  }
};