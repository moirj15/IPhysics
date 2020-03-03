//
// Created by Jimmy on 2/11/2020.
//

#include "VoxelMeshManager.h"

#include "../PhysicsEngine/Settings.h"
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
  mActiveKeys.push_back(h);
  return h;
}

void VoxelMeshManager::SubmitSettings(VMeshHandle handle, Physics::ObjectSettings *settings)
{
  mSettings.emplace(handle, settings);
}

VoxObj::VoxelMesh *VoxelMeshManager::GetMesh(const VMeshHandle handle)
{
  return mMeshes[handle].get();
}

Physics::ObjectSettings *VoxelMeshManager::GetSettings(const VMeshHandle handle)
{
  return mSettings[handle].get();
}

std::vector<MeshTuple> VoxelMeshManager::GetAllMeshes()
{
  std::vector<MeshTuple> tuples;
  for (const auto key : mActiveKeys)
  {
    tuples.emplace_back(key, mMeshes[key].get()->GetMesh(), mSettings[key].get());
  }
  return tuples;
}

VoxelMeshManager::VoxelMeshManager() = default;
VoxelMeshManager::~VoxelMeshManager() = default;
