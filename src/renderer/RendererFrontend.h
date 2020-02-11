#pragma once

#include "../common.h"

#include <glm/mat4x4.hpp>
#include <memory>

class VoxelMesh;
namespace Renderer
{

class RendererBackend;
struct Mesh;
struct Window;

class RendererFrontend
{
  std::unique_ptr<RendererBackend> mBackend;
  glm::mat4 mCamera;
  glm::mat4 mProjection;

public:
  RendererFrontend(Window *window);
  ~RendererFrontend();

  NODISCARD u32 RegisterMeshHandle(const u32 meshHandle);
  NODISCARD u32 RegisterMesh(Mesh *mesh);
  NODISCARD u32 RegisterVoxelMesh(VoxelMesh *vm);
  inline void SetCamera(const glm::mat4 &camera)
  {
    mCamera = camera;
  }
  inline void SetProjection(const glm::mat4 &projection)
  {
    mProjection = projection;
  }
};

} // namespace Renderer