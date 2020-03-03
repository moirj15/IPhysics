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

using VMeshHandle = u32;

class VoxelMeshManager
{
  std::unordered_map<VMeshHandle, std::unique_ptr<VoxObj::VoxelMesh>> mMeshes;
  std::unordered_map<VMeshHandle, std::unique_ptr<Physics::ObjectSettings>> mSettings;

public:
  static VoxelMeshManager &Get();
  VMeshHandle SubmitMesh(VoxObj::VoxelMesh *mesh);
  void SubmitSettings(VMeshHandle handle, Physics::ObjectSettings *settings);
  VoxObj::VoxelMesh *GetMesh(VMeshHandle handle);
  Physics::ObjectSettings *GetSettings(VMeshHandle handle);

  // Delete these to prevent copies
  VoxelMeshManager(VoxelMeshManager &&) = delete;
  VoxelMeshManager &operator=(const VoxelMeshManager &) = delete;

private:
  VoxelMeshManager();
  ~VoxelMeshManager();
};
