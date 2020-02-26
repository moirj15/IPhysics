#pragma once

#include "../common.h"
#include "../utils/VoxelMeshManager.h"

#include <glm/mat4x4.hpp>
#include <memory>

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

public:
  explicit RendererFrontend(Window *window, Camera *camera);
  ~RendererFrontend();

  NODISCARD u32 RegisterMeshHandle(VMeshHandle meshHandle);
  NODISCARD u32 RegisterMesh(Mesh *mesh);
  NODISCARD u32 RegisterVoxelMesh(VoxObj::VoxelMesh *vm);
  inline void SetProjection(const glm::mat4 &projection)
  {
    mProjection = projection;
  }
  void DrawMesh(const u32 handle);
  void Draw();
  void Clear();
  void RemoveMesh(u32 handle);
};

} // namespace Renderer