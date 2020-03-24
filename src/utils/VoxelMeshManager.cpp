//
// Created by Jimmy on 2/11/2020.
//

#include "VoxelMeshManager.h"

#include "../PhysicsEngine/Settings.h"
#include "../voxelObjects/VoxelMesh.h"

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
  mUpdatableSettings.emplace(handle, settings);
  mOriginalSettings.emplace(handle, settings);
}

VoxObj::VoxelMesh *VoxelMeshManager::GetMesh(const VMeshHandle handle)
{
  return mMeshes[handle].get();
}

Physics::ObjectSettings *VoxelMeshManager::GetSettings(const VMeshHandle handle)
{
  return mUpdatableSettings[handle].get();
}

std::vector<MeshTuple> VoxelMeshManager::GetAllMeshes()
{
  std::vector<MeshTuple> tuples;
  for (const auto key : mActiveKeys)
  {
    tuples.emplace_back(key, mMeshes[key].get(), mUpdatableSettings[key].get());
  }
  return tuples;
}

void VoxelMeshManager::UpdateOriginalSettings(
    VMeshHandle handle, const Physics::ObjectSettings &originalSettings)
{
  (*mOriginalSettings[handle].get()) = originalSettings;
  (*mUpdatableSettings[handle].get()) = originalSettings;
}

void VoxelMeshManager::RestoreSettings()
{
  for (auto &[key, _] : mUpdatableSettings)
  {
    (*mUpdatableSettings[key].get()) = (*mOriginalSettings[key].get());
  }
}

VoxelMeshManager::VoxelMeshManager() = default;
VoxelMeshManager::~VoxelMeshManager() = default;
