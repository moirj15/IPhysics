#pragma once

#include "../common.h"
#include "../renderer/mesh.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace VoxObj
{

struct Voxel;

// struct VoxelNeighbor
// {
//   Voxel *mNeighbor;
//   glm::vec3 mAllowedRange;
//   glm::quat mAllowedRotation;
//
//   VoxelNeighbor(Voxel *neighbor, const glm::vec3 &allowedRange, const glm::quat &allowedRotation)
//   :
//       mNeighbor(neighbor), mAllowedRange(allowedRange), mAllowedRotation(allowedRotation)
//   {
//   }
// };

struct Voxel
{
  glm::vec3 mDimensions;
  std::vector<glm::vec3> mNeighbors;
  std::vector<u32> mMeshVerteciess;

  Voxel() = default;

  explicit Voxel(const f32 dimmensions) : mDimensions(dimmensions)
  {
  }

  explicit Voxel(const Voxel &v) :
      mDimensions(v.mDimensions), mNeighbors(v.mNeighbors), mMeshVerteciess(v.mMeshVerteciess)
  {
  }
};

// class VoxelMesh
// {
//   Mesh *mMesh;
//
// public:
//   Mesh *GetMesh() const
//   {
//     return mMesh;
//   }
// };

} // namespace VoxObj