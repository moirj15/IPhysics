//
// Created by Jimmy on 2/11/2020.
//

#include <VoxelObjects/MeshManager.h>

//#include "../PhysicsEngine/Settings.h"

MeshHandle MeshManager::sCurrentHandle = 0;

MeshManager &MeshManager::operator=(const MeshManager &other)
{
  for (const auto &pair : other.mVoxelMeshes) {
    mVoxelMeshes.emplace(pair);
  }
  for (const auto &pair : other.mMeshes) {
    mMeshes.emplace(pair);
  }
  return *this;
}

MeshHandle MeshManager::AddMeshes(objs::Mesh mesh, objs::VoxelMesh voxelMesh)
{
  sCurrentHandle++;
  mMeshes.emplace(sCurrentHandle, mesh);
  mVoxelMeshes.emplace(sCurrentHandle, voxelMesh);
  return sCurrentHandle;
}

MeshHandle MeshManager::AddMesh(objs::Mesh mesh)
{
  sCurrentHandle++;
  mMeshes.emplace(sCurrentHandle, mesh);
  return sCurrentHandle;
}

void MeshManager::AddVoxelMesh(objs::VoxelMesh mesh, MeshHandle handle)
{
  mVoxelMeshes.emplace(handle, mesh);
}

objs::Mesh *MeshManager::GetMesh(MeshHandle handle)
{
  return &mMeshes[handle];
}

objs::VoxelMesh *MeshManager::GetVoxelMesh(MeshHandle handle)
{
  return &mVoxelMeshes[handle];
}

std::vector<objs::Mesh*> MeshManager::GetAllMeshes()
{
  std::vector<objs::Mesh*> ret;
  ret.reserve(mMeshes.size());
  for (auto& [_, mesh] : mMeshes) {
    ret.push_back(&mesh);
  }
  return ret;
}
std::vector<objs::VoxelMesh*> MeshManager::GetVoxelAllMeshes()
{
  std::vector<objs::VoxelMesh*> ret;
  ret.reserve(mVoxelMeshes.size());
  for (auto& [_, voxelMesh] : mVoxelMeshes) {
    ret.push_back(&voxelMesh);
  }
  return ret;
}

std::vector<MeshHandle> MeshManager::GetAllHandles()
{
  std::vector<MeshHandle> ret;
  ret.reserve(mVoxelMeshes.size());
  for (auto& [handle, _] : mVoxelMeshes) {
    ret.push_back(handle);
  }
  return ret;
}
