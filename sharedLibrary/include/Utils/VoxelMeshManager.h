//
// Created by Jimmy on 2/11/2020.
//

#pragma once

#include "../../common.h"

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
using MeshTuple = std::tuple<VMeshHandle, VoxObj::VoxelMesh *, Physics::ObjectSettings *>;
class VoxelMeshManager
{

  std::unordered_map<VMeshHandle, std::unique_ptr<VoxObj::VoxelMesh>> mOriginalMeshes;
  std::unordered_map<VMeshHandle, std::unique_ptr<VoxObj::VoxelMesh>> mMeshes;
  std::unordered_map<VMeshHandle, std::unique_ptr<Physics::ObjectSettings>> mUpdatableSettings;
  std::unordered_map<VMeshHandle, std::unique_ptr<Physics::ObjectSettings>> mOriginalSettings;
  std::vector<VMeshHandle> mActiveKeys;

public:
  /**
   * \brief: Getter for the static instance of the VoxelMeshManager.
   */
  static VoxelMeshManager &Get();

  /**
   * \brief: Submits the given VoxelMesh and returns the VMeshHandle that it will be associated
   * with.
   */
  VMeshHandle SubmitMesh(VoxObj::VoxelMesh *mesh);

  /**
   * \brief: Retrieves a pointer to the VoxelMesh with the given VMeshHandle.
   */
  VoxObj::VoxelMesh *GetMesh(VMeshHandle handle);

  /**
   * \brief: Retrieves the updatable object settings with the given handle.
   */
  Physics::ObjectSettings *GetSettings(VMeshHandle handle);

  /**
   * \brief: Retrieves the contents of the VoxelMeshManager and stores them in a list of tuples
   * consisting of a VMeshHandle, the pointer to the VoxelMesh, and a pointer to the updatable
   * settings.
   */
  std::vector<MeshTuple> GetAllMeshes();

  /**
   * \brief: Updates the original settings with the given handle to the contents of the given
   * settings object.
   */
  void UpdateOriginalSettings(VMeshHandle handle, const Physics::ObjectSettings &originalSettings);
  /**
   * \brief: Restores all the updatable settings to their original state.
   */
  void RestoreSettings();

  // Delete these to prevent copies
  VoxelMeshManager(VoxelMeshManager &&) = delete;
  VoxelMeshManager &operator=(const VoxelMeshManager &) = delete;

private:
  VoxelMeshManager();
  ~VoxelMeshManager();
};
