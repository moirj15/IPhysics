#pragma once

#include "ShaderData.h"

#include <Common.h>
#include <Context.hpp>
#include <Renderer/Camera.h>
#include <Utils/QuickCastBuffer.h>
#include <VoxelObjects/MeshManager.h>
#include <glm/mat4x4.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace shared
{

class Renderer
{
  using BufferHandles = std::pair<focus::VertexBufferHandle, focus::IndexBufferHandle>;
  std::unordered_map<MeshHandle, BufferHandles> mMeshes;
  std::unordered_map<MeshHandle, BufferHandles> mDebugMeshes;
  MeshManager *mMeshManager;

  struct PhongConstants {
    glm::mat4 camera;
    glm::mat4 mvp;
    glm::mat4 normalMat;
    glm::vec4 lightPosition{4.0f, 1.0f, 2.0f, 1.0f};
  };

  struct PhongFragmentConstants {
    const glm::vec4 lightColor{1.0f, 1.0f, 1.0f, 1.0f};
    const glm::vec4 ambientLight{0.5f, 0.5f, 0.5f, 1.0f};
    const glm::vec4 ambientColor{1.0f, 1.0f, 0.7f, 1.0f};
    const glm::vec4 diffuseColor{0.0f, 1.0f, 0.7f, 1.0f};
    const glm::vec4 specularColor{1.0f, 1.0f, 1.0f, 1.0f};
    const glm::vec4 coeff{0.5f, 0.7f, 1.0f, 1.0f};
  } mPhongFragmentconstantsData;

  focus::ConstantBufferDescriptor mPhongVertDesc = {
      .name = "constants",
      .types = {focus::VarType::Mat4, focus::VarType::Mat4, focus::VarType::Mat4, focus::VarType::Vec4},
      .slot = 0,
      .usage = focus::BufferUsage::Static,
      .sizeInBytes = sizeof(PhongConstants),
  };
  focus::ConstantBufferDescriptor mPhongFragDesc = {
      .name = "fragConstants",
      .types = {focus::VarType::Vec4, focus::VarType::Vec4, focus::VarType::Vec4, focus::VarType::Vec4,
          focus::VarType::Vec4, focus::VarType::Vec4},
      .slot = 1,
      .usage = focus::BufferUsage::Static,
      .sizeInBytes = sizeof(PhongFragmentConstants),
  };

  struct LineConstants {
    glm::mat4 mvp;
    glm::vec3 color;
  };

  focus::ShaderHandle mLineShader;
  focus::ConstantBufferHandle mLineConstants;

  focus::ShaderHandle mPhongShader;
  focus::ConstantBufferHandle mPhongConstants;
  focus::ConstantBufferHandle mPhongFragmentConstants;

public:
  explicit Renderer(MeshManager *meshManager) :
      mMeshManager(meshManager), mPhongShader(focus::gContext->CreateShaderFromSource("Phong",
                                     ReadFile("shaders/phongLight.vert"), ReadFile("shaders/phongLight.frag"))),
      mLineShader(focus::gContext->CreateShaderFromSource(
          "Line", ReadFile("shaders/line.vert"), ReadFile("shaders/line.frag"))),
      mPhongConstants(focus::gContext->CreateConstantBuffer(nullptr, sizeof(PhongConstants), mPhongVertDesc)),
      mPhongFragmentConstants(
          focus::gContext->CreateConstantBuffer(&mPhongFragmentconstantsData, sizeof(PhongFragmentConstants), mPhongFragDesc)),
      mLineConstants(focus::gContext->CreateConstantBuffer(nullptr, sizeof(LineConstants),
          {
              .name = "constants",
              .types = {focus::VarType::Mat4, focus::VarType::Vec3},
              .slot = 0,
              .usage = focus::BufferUsage::Static,
              .sizeInBytes = sizeof(LineConstants),
          }))
  {
  }

  /**
   * @brief Load the mesh with the given handle from the MeshManager into vertex buffers
   *        for rendering. Will also load information into a debug vertex buffer for rendering
   *        voxels.
   * @param handle The handle of the mesh that will be loaded.
   */
  void LoadMesh(MeshHandle handle);
  void LoadDebugMesh(MeshHandle handle);

  void DrawMesh(MeshHandle handle, const Camera &camera, const glm::mat4 &model);
  void DrawDebugVoxels(MeshHandle handle, const Camera &camera, const glm::mat4 &model);
  void ClearScreen() { focus::gContext->Clear(); }
  void UpdateScreen(const focus::Window &window) { focus::gContext->SwapBuffers(window); }
};

} // namespace shared