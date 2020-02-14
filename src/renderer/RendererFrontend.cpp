
#include "RendererFrontend.h"

#include "../voxelObjects/voxel.h"
#include "RendererBackend.h"
#include "window.h"

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

void RendererFrontend::DrawMesh(const u32 handle)
{
  DrawCommand dc(CommandType::DrawSolid, handle, 0, nullptr, glm::vec3(1.0f, 0.0f, 0.0f));
  mBackend->SubmitCommand(dc);
}

void RendererFrontend::Draw()
{
  mBackend->Draw(mCamera, mProjection);
}

void RendererFrontend::Clear()
{
  mBackend->ClearCommandQueue();
  mBackend->ClearScreen();
}
} // namespace Renderer