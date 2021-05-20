#pragma once

#include <Common.h>
#include "Objects.h"
#include <memory>
#include <unordered_map>

using MeshHandle = u32;
class MeshManager
{
  std::unordered_map<MeshHandle, objs::Mesh> mMeshes;
  std::unordered_map<MeshHandle, objs::VoxelMesh> mVoxelMeshes;
  static MeshHandle sCurrentHandle;

public:
  MeshManager() = default;
  MeshManager &operator=(const MeshManager &other);
  MeshHandle AddMeshes(objs::Mesh mesh, objs::VoxelMesh voxelMesh);
  MeshHandle AddMesh(objs::Mesh mesh);
  void AddVoxelMesh(objs::VoxelMesh mesh, MeshHandle handle);
  /**
   * \brief: Retrieves a pointer to the VoxelMesh with the given VMeshHandle.
   */
  objs::Mesh *GetMesh(MeshHandle handle);
  objs::VoxelMesh *GetVoxelMesh(MeshHandle handle);

  /**
   * \brief: Retrieves the updatable object settings with the given handle.
   */
  // Physics::ObjectSettings *GetSettings(VMeshHandle handle);

  std::vector<objs::Mesh*> GetAllMeshes();
  std::vector<objs::VoxelMesh*> GetVoxelAllMeshes();
  std::vector<MeshHandle> GetAllHandles();

  /**
   * \brief: Updates the original settings with the given handle to the contents of the given
   * settings object.
   */
  // void UpdateOriginalSettings(VMeshHandle handle, const Physics::ObjectSettings &originalSettings);
  /**
   * \brief: Restores all the updatable settings to their original state.
   */
  // void RestoreSettings();

  // Delete these to prevent copies
  MeshManager(MeshManager &&) = delete;
};
