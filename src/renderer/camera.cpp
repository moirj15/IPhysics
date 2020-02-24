#include "camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

void Camera::Move(const glm::vec3 &velocity)
{
  mRecalculate = true;
  mPosition += (mStrafe * velocity.x) + (mUp * velocity.y) + (mTarget * velocity.z);
}

void Camera::Rotate(glm::vec2 angle)
{
  mRecalculate = true;
  mAngles += glm::radians(angle * 0.005f);
  if (mAngles.y > glm::radians(89.0f))
  {
    mAngles.y = glm::radians(89.0f);
  }
  if (mAngles.y < glm::radians(-89.0f))
  {
    mAngles.y = glm::radians(-89.0f);
  }

  mTarget = glm::normalize(
      glm::vec3(cos(mAngles.y) * sin(mAngles.x), sin(mAngles.y), cos(mAngles.y) * cos(mAngles.x)));
  mStrafe = glm::cross(mTarget, mUp);
}

glm::mat4 Camera::CalculateMatrix()
{
  if (mRecalculate)
  {
    mRecalculate = false;
    mMatrix = glm::lookAt(mPosition, mPosition + mTarget, mUp);
    return mMatrix;
  }
  else
  {
    return mMatrix;
  }
}
