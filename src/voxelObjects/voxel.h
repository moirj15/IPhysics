#pragma once

#include "../common.h"
#include "../renderer/mesh.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace VoxObj
{

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

} // namespace VoxObj