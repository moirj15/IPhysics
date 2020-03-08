#pragma once

#include "../common.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace VoxObj
{

struct Voxel
{
  glm::vec3 mDimensions{0, 0, 0};
  glm::vec3 mPosition{0.0, 0.0, 0.0};
  std::vector<glm::vec3> mNeighbors;
  std::vector<u32> mMeshVertices;

  Voxel() = default;

  explicit Voxel(const f32 dimmensions, const glm::vec3 position) :
      mDimensions(dimmensions), mPosition(position)
  {
  }

  explicit Voxel(const Voxel &v) :
      mDimensions(v.mDimensions), mPosition(v.mPosition), mNeighbors(v.mNeighbors),
      mMeshVertices(v.mMeshVertices)
  {
  }
};

} // namespace VoxObj