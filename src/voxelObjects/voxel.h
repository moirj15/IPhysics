#pragma once

#include "../common.h"
#include "../renderer/mesh.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace vx
{

struct Voxel;

struct VoxelNeighbor
{
  Voxel *mNeighbor;
  glm::vec3 mAllowedRange;
  glm::quat mAllowedRotation;

  VoxelNeighbor(Voxel *neighbor, const glm::vec3 &allowedRange, const glm::quat &allowedRotation) :
      mNeighbor(neighbor), mAllowedRange(allowedRange), mAllowedRotation(allowedRotation)
  {
  }
};

struct Voxel
{
  std::vector<VoxelNeighbor> mNeighbors;
  glm::vec3 mPosition;
  glm::quat mRotation;
  glm::vec3 mPrevPosition;
  glm::quat mPrevRotation;

  Voxel(const glm::vec3 &position) :
      mNeighbors(), mPosition(position), mRotation(), mPrevPosition(position),
      mPrevRotation(mRotation)
  {
  }
};

class VoxelMesh
{
  Mesh *mMesh;
public:
  Mesh *GetMesh() const { return mMesh; }
};

} // namespace vx