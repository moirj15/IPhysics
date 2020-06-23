
#include <Renderer/RendererFrontend.h>

//#include "../../PhysicsEngine/Settings.h"
//#include "../../voxelObjects/VoxelMesh.h"
#include <Renderer/RendererBackend.h>
#include <Renderer/ShaderData.h>
#include <Renderer/Camera.h>
#include <Renderer/Mesh.h>
#include <Renderer/Window.h>

#include <glm/gtx/norm.hpp>
#include <glm/gtx/transform.hpp>
#include <VoxelObjects/VoxelMesh.h>
namespace Renderer
{

RendererFrontend::RendererFrontend(Window *window, Camera *camera) :
    mBackend(new RendererBackend(window)), mCamera(camera), mProjection(1.0f)
{
}

RendererFrontend::~RendererFrontend() = default;

//void RendererFrontend::RegisterMeshHandle(const VMeshHandle voxelMeshHandle)
//{
////  auto &meshManager = VoxelMeshManager::Get();
////  auto *voxelMesh = meshManager.GetMesh(voxelMeshHandle);
////  auto meshHandle = RegisterMesh(voxelMesh->GetMesh());
////  mMeshHandles.emplace(voxelMeshHandle, meshHandle);
//}

u32 RendererFrontend::RegisterMesh(Mesh *mesh)
{
  // Calculate our own normals using the mesh vertex positions
  auto &vertices = mesh->mVertices;
  auto &normals = mesh->mNormals;
  normals.SetBufferSize(vertices.BufferSize());
  for (u32 i = 0; i < mesh->mVertices.CastBufferSize(); i++)
  {
    auto &vertices = mesh->mVertices;
    auto &normals = mesh->mNormals;
    auto normal = mesh->mVertices.AccessCastBuffer(i);
    //     glm::vec3 normal(vertices[i], vertices[i + 1], vertices[i + 2]);
    if (glm::length(normal) > 0.0f)
    {
      normal = glm::normalize(normal);
    }
    normals.AccessCastBuffer(i) = normal;
    //     normals[i] = normal.x;
    //     normals[i + 1] = normal.y;
    //     normals[i + 2] = normal.z;
  }
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
  const glm::vec3 voxelNormals[] = {
      glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),   // 0 Front
      glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f)),  // 1
      glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)), // 2
      glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)),  // 3

      glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)),   // 4 Back
      glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)),  // 5
      glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)), // 6
      glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)),  // 7
  };
  // do this super inefficiently
  const auto voxelSize = vm->GetInitialVoxelSize();
  for (const auto &[key, voxel] : vm->mVoxels)
  {
    const glm::vec3 voxelVerts[] = {
        voxel.mPosition + (glm::vec3(1.0f, 1.0f, 1.0f) * (voxelSize / 2.0f)),   // 0 Front
        voxel.mPosition + (glm::vec3(-1.0f, 1.0f, 1.0f) * (voxelSize / 2.0f)),  // 1
        voxel.mPosition + (glm::vec3(-1.0f, -1.0f, 1.0f) * (voxelSize / 2.0f)), // 2
        voxel.mPosition + (glm::vec3(1.0f, -1.0f, 1.0f) * (voxelSize / 2.0f)),  // 3

        voxel.mPosition + (glm::vec3(1.0f, 1.0f, -1.0f) * (voxelSize / 2.0f)),   // 4 Back
        voxel.mPosition + (glm::vec3(-1.0f, 1.0f, -1.0f) * (voxelSize / 2.0f)),  // 5
        voxel.mPosition + (glm::vec3(-1.0f, -1.0f, -1.0f) * (voxelSize / 2.0f)), // 6
        voxel.mPosition + (glm::vec3(1.0f, -1.0f, -1.0f) * (voxelSize / 2.0f)),  // 7
    };
    u32 indexOffset = mesh.mVertices.BufferSize() / 3;
    for (u32 index : faceIndecies)
    {
      mesh.mIndices.push_back(index + indexOffset);
    }
    //     auto transform = glm::translate((voxel.mPosition));
    for (u32 i = 0; i < ArraySize(voxelVerts); i++)
    {
      const auto &vert = voxelVerts[i];
      mesh.mVertices.CastBufferPushBack(voxelVerts[i]);
      mesh.mNormals.CastBufferPushBack(voxelNormals[i]);
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

void RendererFrontend::DrawPoints(const QuickCastBuffer<f32, glm::vec3> &points)
{
  static u32 handle = 0;
  std::vector<ShaderData> shaderData = {
      ShaderData("camera", mCamera->CalculateMatrix()),
      ShaderData("projection", mProjection),
  };
  Mesh mesh;
  mesh.mVertices = points;
  for (u32 i = 0; i < points.CastBufferSize(); i++)
  {
    mesh.mIndices.emplace_back(i);
  }

  mBackend->RemoveMesh(handle);
  handle = mBackend->SubmitMesh(&mesh);
  //   mPointMeshHandles.emplace_back(handle);
  DrawCommand dc(CommandType::DrawPoints, handle, shaderData);
  mBackend->SubmitCommand(dc);
}

void RendererFrontend::Draw()
{
//  for (const auto &[key, mesh, settings] : VoxelMeshManager::Get().GetAllMeshes())
//  {
//    glm::mat4 transform = glm::translate(settings->mPosition);
//    auto modelView = mCamera->CalculateMatrix() * transform;
//    auto projModelView = mProjection * modelView;
//    auto normalMat = glm::inverse(glm::transpose(glm::mat3(modelView)));
//    // auto eye_lightPosition = mCamera->CalculateMatrix() *
//    // std::vector<ShaderData> shaderData = {
//    //    ShaderData("camera", mCamera->CalculateMatrix()),    ShaderData("projection",
//    //    mProjection), ShaderData("lightPosition", mCamera->GetPosition()), ShaderData("transform",
//    //    transform), ShaderData("color", glm::vec3(1.0f, 0.0f, 0.0f)),
//    //};
//    std::vector<ShaderData> shaderData = {
//        ShaderData("camera", mCamera->CalculateMatrix()),
//        ShaderData("projModelView", projModelView),
//        ShaderData("normalMat", normalMat),
//        ShaderData("lightPosition", mCamera->GetPosition()),
//        ShaderData("transform", transform),
//        ShaderData("lightColor", glm::vec3(1.0f)),
//        ShaderData("ambientLight", glm::vec3(0.3f, 0.5f, 0.0f)),
//        ShaderData("ambientColor", glm::vec3(0.3f, 0.5f, 0.0f)),
//        ShaderData("diffuseColor", glm::vec3(0.7f, 0.8f, 0.0f)),
//        ShaderData("specularColor", glm::vec3(0.0f, 1.0f, 1.0f)),
//        ShaderData("coeff", glm::vec4(0.5f, 0.5f, 0.5f, 10.0f)),
//    };
//    DrawCommand dc(CommandType::DrawSolidPhong, mMeshHandles[key], shaderData);
//    mBackend->SubmitCommand(dc);
//  }
//  //   for (const u32 h : mPointMeshHandles)
//  //   {
//  //     std::vector<ShaderData> shaderData = {
//  //         ShaderData("projection", mProjection),
//  //         ShaderData("camera", mCamera->CalculateMatrix()),
//  //     };
//  //     DrawCommand dc(CommandType::DrawPoints, h, shaderData);
//  //     mBackend->SubmitCommand(dc);
//  //   }
//  mBackend->Draw();
}

void RendererFrontend::Clear()
{
  // mBackend->ClearCommandQueue();
  mBackend->ClearScreen();
}

void RendererFrontend::RemoveMesh(u32 handle)
{
  mBackend->RemoveMesh(handle);
}

void RendererFrontend::UpdateMesh(const VMeshHandle handle)
{
//  mBackend->UpdateMesh(mMeshHandles[handle], VoxelMeshManager::Get().GetMesh(handle)->mMesh);
}

} // namespace Renderer