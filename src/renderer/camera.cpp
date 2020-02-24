#include "camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

void Camera::Move(const glm::vec3 &velocity)
{
  mRecalculate = true;
  mPosition += mTarget * velocity.z;
  mPosition += mStrafe * velocity.x;
}

void Camera::Rotate(glm::vec2 angle)
{
  mRecalculate = true;
  mAngles += glm::radians(angle * 0.05f);
  if (mAngles.y > glm::radians(89.0f))
  {
    mAngles.y = glm::radians(89.0f);
  }
  if (mAngles.y < glm::radians(-89.0f))
  {
    mAngles.y = glm::radians(-89.0f);
  }

  // mTarget.x = glm::cos(glm::radians(mAngles.x)) * glm::cos(glm::radians(mAngles.y));
  // mTarget.y = glm::sin(glm::radians(mAngles.y));
  // mTarget.z = glm::sin(glm::radians(mAngles.x)) * glm::cos(glm::radians(mAngles.y));
  mTarget =
      glm::rotate(glm::rotate(mAngles.x, X_AXIS), mAngles.y, Y_AXIS) * glm::vec4(mTarget, 1.0f);
  mTarget = glm::normalize(mTarget);
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
