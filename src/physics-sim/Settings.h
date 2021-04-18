#pragma once

#include <Common.h>

#include <glm/vec3.hpp>

namespace Physics
{

struct ObjectSettings
{
  glm::vec3 mPosition{0.0f, 0.0f, 0.0f};
  glm::vec3 mInitialVelocity{0.0f, 0.0f, 0.0f};
  glm::vec3 mInitialAccleration{0.0f, 0.0f, 0.0f};
  // For the entire object, have to divide by the number of voxels to get voxel mass
  f32 mMass{0.0f};
  bool mTreatAsRigid{false}; // Doesn't do the soft body physics simulation if True
};

struct EngineSettings
{
  bool mEnableGravity{false};
  bool mEnableExtension{false};
  f32 mMouseForce{0.0f};
};

} // namespace Physics