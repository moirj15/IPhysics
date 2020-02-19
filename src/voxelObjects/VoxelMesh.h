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

// class VoxelPlane
// {
//   u32 mWidth;
//   u32 mHeight;
//   std::vector<std::vector<Voxel>> mVoxels;
//
// public:
//   VoxelPlane(const u32 width, const u32 height) :
//       mWidth(width), mHeight(height), mVoxels(mWidth, std::vector<Voxel>(mHeight, Voxel()))
//   {
//   }
//
//   NODISCARD inline Voxel GetVoxel(const glm::ivec2 &position) const
//   {
//     return mVoxels[position.x][position.y];
//   }
//
//   NODISCARD inline u32 GetWidth() const
//   {
//     return mWidth;
//   }
//   NODISCARD inline u32 GetHeight() const
//   {
//     return mHeight;
//   }
//
//   NODISCARD inline std::vector<Voxel> &operator[](const u32 index)
//   {
//     return mVoxels[index];
//   }
// };

class VoxelMesh
{
  glm::ivec3 mExtentsVoxelSpace;
  glm::vec3 mExtentsObjectSpace;
  std::unordered_map<glm::ivec3, Voxel> mVoxels;
  Mesh *mMesh;

public:
  VoxelMesh(const glm::ivec3 extentsVoxelSpace, const glm::vec3 &extentsObjectSpace, Mesh *mesh) :
      mExtentsVoxelSpace(extentsVoxelSpace), mExtentsObjectSpace(extentsObjectSpace), mMesh(mesh)
  {
  }

  NODISCARD inline const Voxel &GetVoxel(const glm::ivec3 &position)
  {
    assert(glm::all(glm::lessThan(position, mExtentsVoxelSpace)));
    return mVoxels[position];
  }
  inline void SetVoxel(const glm::ivec3 &position, const Voxel &voxel)
  {
    assert(glm::all(glm::lessThan(position, mExtentsVoxelSpace)));
    mVoxels.emplace(position, voxel);
  }

  NODISCARD inline Mesh *GetMesh() const
  {
    return mMesh;
  }
};

} // namespace VoxObj