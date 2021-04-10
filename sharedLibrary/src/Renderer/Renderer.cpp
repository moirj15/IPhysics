#include <Renderer/Mesh.h>
#include <Renderer/Renderer.h>
#include <Renderer/Shader.h>
#include <Renderer/Window.h>
#include <VoxelObjects/VoxelMesh.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <Renderer/Camera.h>

namespace shared
{
void Renderer::LoadMesh(MeshHandle handle)
{
  auto *mesh = mMeshManager->GetMesh(handle);
  if (!mMeshes.contains(handle)) {
    auto buffer = mesh->GetInterleved();
    // clang-format off
    auto vbHandle = focus::gContext->CreateVertexBuffer((void*)buffer.data(), buffer.size() * sizeof(f32), {
      .inputDescriptorName = {"vPosition", "vNormal"},
      .bufferType = focus::BufferType::InterLeaved,// TODO: switch to interleved data
      .types = {focus::VarType::Vec3, focus::VarType::Vec3},
      .sizeInBytes = mesh->VerticesSizeInBytes(),
      .elementSizeInBytes = 24,
      .usage = focus::BufferUsage::Static,
    });
    auto ibHandle = focus::gContext->CreateIndexBuffer((void*)mesh->indices.data(), mesh->IndicesSizeInBytes(), {
      .type = focus::IndexBufferType::U32,
      .sizeInBytes = mesh->IndicesSizeInBytes(),
      .usage = focus::BufferUsage::Static,
    });
    // clang-format on
    mMeshes[handle] = {vbHandle, ibHandle};
  } else {
    auto [vbHandle, ibHandle] = mMeshes[handle];
    focus::gContext->UpdateVertexBuffer(vbHandle, (void *)mesh->vertices.data(), mesh->VerticesSizeInBytes());
    focus::gContext->UpdateIndexBuffer(vbHandle, (void *)mesh->indices.data(), mesh->IndicesSizeInBytes());
  }
}

// TODO: it would be better to do this in a compute shader (and pretty straight forward too).
// But other things have a higher priority so this will have to wait
void Renderer::LoadDebugMesh(MeshHandle handle)
{
  auto *voxelMesh = mMeshManager->GetVoxelMesh(handle);
  u32 baseIndices[] = {
      0, 1, // top front left - top back left
      1, 2, // top back left - top back right
      2, 3, // top back right - top front right
      3, 0, // top front right - tob front left

      4, 5, // bottom front left - bottom back left
      5, 6, // bottom back left - bottom back right
      6, 7, // bottom back right - bottom front right
      7, 4, // bottom front right - bottom front left

      0, 4, // top front left - bottom front left
      1, 5, // top back left - bottom back left
      2, 6, // top back right - bottom back right
      3, 7, // top front right - bottom front right
  };

  // clang-format off
  glm::vec3 baseVertices[] = {
      glm::vec3(-1.0f,  1.0f,  1.0f), // top front left
      glm::vec3(-1.0f,  1.0f, -1.0f), // top back left
      glm::vec3( 1.0f,  1.0f, -1.0f), // top back right
      glm::vec3( 1.0f,  1.0f,  1.0f), // top front right

      glm::vec3(-1.0f, -1.0f,  1.0f), // top front left
      glm::vec3(-1.0f, -1.0f, -1.0f), // top back left
      glm::vec3( 1.0f, -1.0f, -1.0f), // top back right
      glm::vec3( 1.0f, -1.0f,  1.0f), // top front right
  };
  // clang-format on

  std::vector<u32> indices;
  std::vector<f32> vertices;
  u32 currentIndex = 0;
  const auto voxelSize = voxelMesh->initialVoxelSize;
  for (const auto &[key, voxel] : voxelMesh->voxels) {
    const glm::vec3 voxelVerts[] = {
        voxel.position + (glm::vec3(1.0f, 1.0f, 1.0f) * (voxelSize / 2.0f)),   // 0 Front
        voxel.position + (glm::vec3(-1.0f, 1.0f, 1.0f) * (voxelSize / 2.0f)),  // 1
        voxel.position + (glm::vec3(-1.0f, -1.0f, 1.0f) * (voxelSize / 2.0f)), // 2
        voxel.position + (glm::vec3(1.0f, -1.0f, 1.0f) * (voxelSize / 2.0f)),  // 3

        voxel.position + (glm::vec3(1.0f, 1.0f, -1.0f) * (voxelSize / 2.0f)),   // 4 Back
        voxel.position + (glm::vec3(-1.0f, 1.0f, -1.0f) * (voxelSize / 2.0f)),  // 5
        voxel.position + (glm::vec3(-1.0f, -1.0f, -1.0f) * (voxelSize / 2.0f)), // 6
        voxel.position + (glm::vec3(1.0f, -1.0f, -1.0f) * (voxelSize / 2.0f)),  // 7
    };
    for (u32 i = 0; i < ArraySize(baseIndices); i++) {
      indices.push_back(baseIndices[i] + currentIndex);
    }
    currentIndex += 8;
    for (u32 i = 0; i < ArraySize(voxelVerts); i++) {
      vertices.push_back(voxelVerts[i].x);
      vertices.push_back(voxelVerts[i].y);
      vertices.push_back(voxelVerts[i].z);
    }
  }

  if (!mDebugMeshes.contains(handle)) {
    // clang-format off
    auto vbHandle = focus::gContext->CreateVertexBuffer((void *)vertices.data(), vertices.size() * sizeof(f32), {
        .inputDescriptorName = {"vPosition"},
        .bufferType = focus::BufferType::SingleType, 
        .types = {focus::VarType::Vec3},
        .sizeInBytes = (u32)vertices.size() * sizeof(f32),
        .elementSizeInBytes = 12,
        .usage = focus::BufferUsage::Static,
    });
    auto ibHandle = focus::gContext->CreateIndexBuffer((void *)indices.data(), indices.size() * sizeof(u32), {
        .type = focus::IndexBufferType::U32,
        .sizeInBytes = (u32)indices.size() * sizeof(u32),
        .usage = focus::BufferUsage::Static,
    });
    mDebugMeshes[handle] = {vbHandle, ibHandle};
    // clang-format on
  } else {
    auto [vbHandle, ibHandle] = mMeshes[handle];
    focus::gContext->UpdateVertexBuffer(vbHandle, (void *)vertices.data(), vertices.size() * sizeof(f32));
    focus::gContext->UpdateIndexBuffer(vbHandle, (void *)indices.data(), indices.size() * sizeof(u32));
  }
}

void Renderer::DrawMesh(MeshHandle handle, const Camera &camera, const glm::mat4 &model)
{
  auto cameraMat = camera.CalculateMatrix();
  auto projection = glm::perspective(90.0f, 16.0f / 9.0f, 0.1f, 100.0f);
  auto modelView = cameraMat * model;
  auto normalMat = glm::transpose(glm::inverse(modelView));
  // clang-format off
  PhongConstants constants = {
    .camera = cameraMat,
    .mvp = projection * modelView,
    .normalMat = normalMat,
  };
  focus::gContext->UpdateConstantBuffer(mPhongConstants, (void*)&constants, sizeof(PhongConstants));
  const auto &[vbHandle, ibHandle] = mMeshes[handle];
  focus::gContext->Draw(focus::Primitive::Triangles, {}, mPhongShader, {
    .vbHandles = {vbHandle},
    .cbHandles = {mPhongConstants},
    .ibHandle = ibHandle,
    .indexed = true,
  });
  // clang-format on
}

void Renderer::DrawDebugVoxels(MeshHandle handle, const Camera &camera, const glm::mat4 &model)
{
}

} // namespace shared
