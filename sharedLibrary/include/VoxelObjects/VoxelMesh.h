#pragma once

#include <Renderer/Mesh.h>
#include "Voxel.h"

#include <array>
#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <vector>

namespace VoxObj
{

#if 0
// TODO: Refactor, since the accessors aren't needed anymore
struct VoxelMesh
{
  glm::uvec3 mExtentsVoxelSpace;
  glm::vec3 mExtentsObjectSpace;
  glm::vec3 mInitialVoxelSize;
  std::unordered_map<glm::uvec3, Voxel> mVoxels;
  Mesh *mMesh;

  VoxelMesh(
      const glm::uvec3 extentsVoxelSpace, const glm::vec3 &extentsObjectSpace,
      const glm::vec3 initialVoxelSize, Mesh *mesh) :
      mExtentsVoxelSpace(extentsVoxelSpace),
      mExtentsObjectSpace(extentsObjectSpace), mInitialVoxelSize(initialVoxelSize), mMesh(mesh)
  {
  }
  VoxelMesh(const VoxelMesh &v) :
      mExtentsVoxelSpace(v.mExtentsVoxelSpace), mExtentsObjectSpace(v.mExtentsObjectSpace),
      mVoxels(v.mVoxels), mInitialVoxelSize(v.mInitialVoxelSize), mMesh(new Mesh(*v.mMesh))
  {
  }

  // TODO: remove
  NODISCARD inline const Voxel &GetVoxel(const glm::uvec3 &position)
  {
    assert(glm::all(glm::lessThan(position, mExtentsVoxelSpace)));
    return mVoxels[position];
  }

  inline void SetVoxel(const glm::uvec3 &position, const Voxel &voxel)
  {
    assert(glm::all(glm::lessThan(position, mExtentsVoxelSpace)));
    mVoxels.emplace(position, voxel);
  }

  NODISCARD inline Mesh *GetMesh() const
  {
    return mMesh;
  }
  NODISCARD inline const glm::uvec3 &GetExtentsVoxelSpace() const
  {
    return mExtentsVoxelSpace;
  }

  NODISCARD inline glm::vec3 GetInitialVoxelSize() const
  {
    return mInitialVoxelSize;
  }

  NODISCARD inline bool IsVoxelPresent(const glm::ivec3 &position) const
  {
    return mVoxels.find(position) != mVoxels.end();
  }

  NODISCARD inline const glm::vec3 &GetExtentsObjectSpace() const
  {
    return mExtentsObjectSpace;
  }

  NODISCARD inline u64 GetVoxelCount() const
  {
    return mVoxels.size();
  }
};
#endif

} // namespace VoxObj