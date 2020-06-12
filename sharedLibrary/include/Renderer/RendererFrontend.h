#pragma once

#include "../../common.h"
#include "../utils/QuickCastBuffer.h"
#include "../utils/VoxelMeshManager.h"

#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

class Camera;
struct Window;
struct Mesh;

namespace VoxObj
{
class VoxelMesh;
}
namespace Renderer
{

class RendererBackend;

class RendererFrontend
{
  std::unique_ptr<RendererBackend> mBackend;
  Camera *mCamera;
  glm::mat4 mProjection;
  std::unordered_map<VMeshHandle, u32> mMeshHandles;
  std::vector<u32> mPointMeshHandles;

public:
  explicit RendererFrontend(Window *window, Camera *camera);
  ~RendererFrontend();

  void RegisterMeshHandle(VMeshHandle voxelMeshHandle);
  NODISCARD u32 RegisterMesh(Mesh *mesh);
  NODISCARD u32 RegisterVoxelMesh(VoxObj::VoxelMesh *vm);
  inline void SetProjection(const glm::mat4 &projection)
  {
    mProjection = projection;
  }
  void DrawMesh(const u32 handle);
  void DrawPoints(const QuickCastBuffer<f32, glm::vec3> &points);
  void Draw();
  void Clear();
  void RemoveMesh(u32 handle);
  void UpdateMesh(const VMeshHandle handle);
  RendererBackend *GetBackend()
  {
    return mBackend.get();
  }
};

} // namespace Renderer