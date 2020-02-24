//
// Created by Jimmy on 2/11/2020.
//

#include "VoxelMeshManager.h"

#include "../voxelObjects/VoxelMesh.h"

struct ObjectSettings
{
};

VoxelMeshManager &VoxelMeshManager::Get()
{
  static VoxelMeshManager meshManager;
  return meshManager;
}

VMeshHandle VoxelMeshManager::SubmitMesh(VoxObj::VoxelMesh *mesh)
{
  static VMeshHandle h = 0;
  h++;
  mMeshes.emplace(h, mesh);
  return h;
}

void VoxelMeshManager::SubmitSettings(VMeshHandle handle, ObjectSettings *settings)
{
  mSettings.emplace(handle, settings);
}

VoxObj::VoxelMesh *VoxelMeshManager::GetMesh(const VMeshHandle handle)
{
  return mMeshes[handle].get();
}

ObjectSettings *VoxelMeshManager::GetSettings(const VMeshHandle handle)
{
  return mSettings[handle].get();
}

VoxelMeshManager::VoxelMeshManager() = default;
VoxelMeshManager::~VoxelMeshManager() = default;