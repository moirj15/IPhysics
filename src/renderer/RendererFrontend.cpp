
#include "RendererFrontend.h"

#include "../voxelObjects/VoxelMesh.h"
#include "RendererBackend.h"
#include "ShaderData.h"
#include "camera.h"
#include "window.h"

#include <glm/gtx/transform.hpp>
namespace Renderer
{

RendererFrontend::RendererFrontend(Window *window, Camera *camera) :
    mBackend(new RendererBackend(window)), mCamera(camera), mProjection(1.0f)
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

u32 RendererFrontend::RegisterVoxelMesh(VoxObj::VoxelMesh *vm)
{
  Mesh mesh;
  const u32 faceIndecies[] = {
      0, 1, 2, // Front
      0, 2, 3, //
      5, 4, 7, // Back
      5, 7, 6, //
      1, 5, 6, // Left
      1, 6, 2, //
      4, 0, 3, // Right
      4, 3, 7, //
      4, 5, 1, // Top
      4, 1, 0, //
      6, 7, 3, // Bottom
      6, 3, 2, //
  };
  const glm::vec3 voxelVerts[] = {
      glm::vec3(1.0f, 1.0f, 1.0f),   // 0 Front
      glm::vec3(-1.0f, 1.0f, 1.0f),  // 1
      glm::vec3(-1.0f, -1.0f, 1.0f), // 2
      glm::vec3(1.0f, -1.0f, 1.0f),  // 3

      glm::vec3(1.0f, 1.0f, -1.0f),   // 4 Back
      glm::vec3(-1.0f, 1.0f, -1.0f),  // 5
      glm::vec3(-1.0f, -1.0f, -1.0f), // 6
      glm::vec3(1.0f, -1.0f, -1.0f),  // 7
  };
  // do this super inefficiently
  const auto voxelSize = vm->GetVoxelSize();
  for (const auto &[key, voxel] : vm->GetVoxels())
  {
    glm::vec3 position(key);
    u32 indexOffset = mesh.mVertecies.size() / 3;
    for (u32 index : faceIndecies)
    {
      mesh.mIndecies.push_back(index + indexOffset);
    }
    glm::vec3 vertOffset(position * voxelSize);
    auto transform = glm::translate(glm::scale(voxelSize), position);
    for (const auto &vert : voxelVerts)
    {
      auto transformedVert = transform * glm::vec4(vert, 1.0f);
      mesh.mVertecies.push_back(transformedVert.x);
      mesh.mVertecies.push_back(transformedVert.y);
      mesh.mVertecies.push_back(transformedVert.z);
    }
  }

  return mBackend->SubmitMesh(&mesh);
}

void RendererFrontend::DrawMesh(const u32 handle)
{
  std::vector<ShaderData> shaderData = {
      ShaderData("camera", mCamera->CalculateMatrix()),
      ShaderData("projection", mProjection),
      ShaderData("lightPosition", mCamera->GetPosition()),
  };
  DrawCommand dc(CommandType::DrawSolidFlatShade, handle, shaderData);
  mBackend->SubmitCommand(dc);
}

void RendererFrontend::Draw()
{
  mBackend->Draw();
}

void RendererFrontend::Clear()
{
  mBackend->ClearCommandQueue();
  mBackend->ClearScreen();
}

void RendererFrontend::RemoveMesh(u32 handle)
{
  mBackend->RemoveMesh(handle);
}
} // namespace Renderer