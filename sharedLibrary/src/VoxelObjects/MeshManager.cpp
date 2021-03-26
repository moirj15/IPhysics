//
// Created by Jimmy on 2/11/2020.
//

#include <VoxelObjects/MeshManager.h>

//#include "../PhysicsEngine/Settings.h"

MeshHandle MeshManager::sCurrentHandle = 0;

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

std::vector<objs::Mesh> MeshManager::GetAllMeshes()
{
  assert(0);
  return {};
}
std::vector<objs::VoxelMesh> MeshManager::GetVoxelAllMeshes()
{
  assert(0);
  return {};
}
