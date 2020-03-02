#pragma once

#include "voxel.h"

#include <array>
#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <vector>

struct Mesh;

namespace VoxObj
{

class VoxelMesh
{
  glm::uvec3 mExtentsVoxelSpace;
  glm::vec3 mExtentsObjectSpace;
  std::unordered_map<glm::uvec3, Voxel> mVoxels;
  Mesh *mMesh;

public:
  VoxelMesh(const glm::uvec3 extentsVoxelSpace, const glm::vec3 &extentsObjectSpace, Mesh *mesh) :
      mExtentsVoxelSpace(extentsVoxelSpace), mExtentsObjectSpace(extentsObjectSpace), mMesh(mesh)
  {
  }
  VoxelMesh(VoxelMesh &&v) :
      mExtentsVoxelSpace(std::move(v.mExtentsVoxelSpace)),
      mExtentsObjectSpace(std::move(v.mExtentsObjectSpace)), mVoxels(std::move(v.mVoxels)),
      mMesh(std::move(v.mMesh))
  {
  }

  NODISCARD inline const Voxel &GetVoxel(const glm::uvec3 &position)
  {
    assert(glm::all(glm::lessThan(position, mExtentsVoxelSpace)));
    return mVoxels[position];
  }

  NODISCARD inline std::unordered_map<glm::uvec3, Voxel> GetVoxels() const
  {
    return mVoxels;
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

  NODISCARD inline glm::vec3 GetVoxelSize() const
  {
    auto extents = glm::vec3(mExtentsVoxelSpace);
    return glm::vec3(1.0 / (extents.x));
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

} // namespace VoxObj