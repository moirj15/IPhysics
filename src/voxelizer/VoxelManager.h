#pragma once

#include <Common.h>
#include <unordered_map>
#include <memory>
#include <VoxelObjects/VoxelMesh.h>
#include <Renderer/Mesh.h>

using VoxelHandle = u32;

class VoxelManager
{
  std::unordered_map<VoxelHandle, VoxObj::VoxelMesh> mVoxelMeshes;
  std::unordered_map<VoxelHandle, Mesh> mMeshes;
  VoxelHandle mNextHandle = 0;
public:
  inline VoxelHandle CreateMesh(Mesh &&mesh) {
    VoxelHandle ret = mNextHandle++;
    mMeshes.emplace(ret, mesh);
    return ret;
  }

  inline void SetMesh(VoxelHandle handle, const Mesh &mesh) {
    mMeshes.emplace(handle, mesh);
  }

  inline void SetVoxelMesh(VoxelHandle handle, const VoxObj::VoxelMesh &voxelMesh) {
    mVoxelMeshes.emplace(handle, voxelMesh);
  }

  inline void SetMeshes(VoxelHandle handle, const Mesh &mesh, const VoxObj::VoxelMesh &voxelMesh) {
    mMeshes.emplace(handle, mesh);
    mVoxelMeshes.emplace(handle, voxelMesh);
  }

};