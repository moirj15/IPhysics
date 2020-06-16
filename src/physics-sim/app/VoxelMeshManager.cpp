//
// Created by Jimmy on 2/11/2020.
//

#include "VoxelMeshManager.h"

#include "../PhysicsEngine/Settings.h"
#include <VoxelObjects/VoxelMesh.h>

VoxelMeshManager &VoxelMeshManager::Get()
{
  static VoxelMeshManager meshManager;
  return meshManager;
}

VMeshHandle VoxelMeshManager::SubmitMesh(VoxObj::VoxelMesh *mesh)
{
  static VMeshHandle h = 0;
  h++;
  mOriginalMeshes.emplace(h, mesh);
  mMeshes.emplace(h,  new VoxObj::VoxelMesh(*mesh));
  mActiveKeys.push_back(h);
  mUpdatableSettings.emplace(h, new Physics::ObjectSettings());
  mOriginalSettings.emplace(h, new Physics::ObjectSettings());
  return h;
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
    mMeshes[key].reset(new VoxObj::VoxelMesh(*mOriginalMeshes[key].get()));
    (*mUpdatableSettings[key].get()) = (*mOriginalSettings[key].get());
  }
}

VoxelMeshManager::VoxelMeshManager() = default;
VoxelMeshManager::~VoxelMeshManager() = default;
