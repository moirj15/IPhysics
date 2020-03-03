//
// Created by Jimmy on 2/11/2020.
//

#pragma once

#include "../common.h"

#include <memory>
#include <unordered_map>
namespace VoxObj
{
class VoxelMesh;
}
namespace Physics
{
struct ObjectSettings;
}

struct Mesh;

using VMeshHandle = u32;
using MeshTuple = std::tuple<VMeshHandle, Mesh *, Physics::ObjectSettings *>;
class VoxelMeshManager
{
  std::unordered_map<VMeshHandle, std::unique_ptr<VoxObj::VoxelMesh>> mMeshes;
  std::unordered_map<VMeshHandle, std::unique_ptr<Physics::ObjectSettings>> mSettings;
  std::vector<VMeshHandle> mActiveKeys;

public:
  static VoxelMeshManager &Get();
  VMeshHandle SubmitMesh(VoxObj::VoxelMesh *mesh);
  void SubmitSettings(VMeshHandle handle, Physics::ObjectSettings *settings);
  VoxObj::VoxelMesh *GetMesh(VMeshHandle handle);
  Physics::ObjectSettings *GetSettings(VMeshHandle handle);
  std::vector<MeshTuple> GetAllMeshes();

  // Delete these to prevent copies
  VoxelMeshManager(VoxelMeshManager &&) = delete;
  VoxelMeshManager &operator=(const VoxelMeshManager &) = delete;

private:
  VoxelMeshManager();
  ~VoxelMeshManager();
};
