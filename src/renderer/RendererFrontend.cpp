
#include "RendererFrontend.h"

#include "RendererBackend.h"

namespace Renderer
{

RendererFrontend::RendererFrontend(Window *window) :
    mBackend(new RendererBackend(window)), mCamera(1.0f), mProjection(1.0f)
{
}

RendererFrontend::~RendererFrontend() = default;

u32 RendererFrontend::RegisterMeshHandle(const u32 meshHandle)
{
  IMPLEMENTME;
  return 0;
}

u32 RendererFrontend::RegisterMesh(Mesh *mesh)
{
  IMPLEMENTME;
  return 0;
}

u32 RendererFrontend::RegisterVoxelMesh(VoxelMesh *vm)
{
  IMPLEMENTME;
  return 0;
}

} // namespace Renderer