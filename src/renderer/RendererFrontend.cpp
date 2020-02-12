
#include "RendererFrontend.h"

#include "RendererBackend.h"
#include "window.h"
#include "../voxelObjects/voxel.h"

namespace Renderer
{

RendererFrontend::RendererFrontend(Window *window) :
    mBackend(new RendererBackend(window)), mCamera(1.0f), mProjection(1.0f)
{
}

RendererFrontend::~RendererFrontend() = default;

u32 RendererFrontend::RegisterMeshHandle(const VMeshHandle meshHandle)
{
  auto &meshManager = VoxelMeshManager::Get();
  auto *voxelMesh = meshManager.GetMesh(meshHandle);
  return RegisterMesh(voxelMesh->GetMesh());
}

u32 RendererFrontend::RegisterMesh(Mesh *mesh)
{
  return mBackend->SubmitMesh(mesh);
}

u32 RendererFrontend::RegisterVoxelMesh(VoxelMesh *vm)
{
  IMPLEMENTME();
  return 0;
}
void RendererFrontend::Draw(VMeshHandle handle)
{
}

} // namespace Renderer