#pragma once

#include "ShaderData.h"

#include <Common.h>
#include <Context.hpp>
#include <Utils/QuickCastBuffer.h>
#include <VoxelObjects/MeshManager.h>
#include <glm/mat4x4.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include <Renderer/Camera.h>

namespace shared
{

class Renderer
{
  using BufferHandles = std::pair<focus::VertexBufferHandle, focus::IndexBufferHandle>;
  std::unordered_map<MeshHandle, BufferHandles> mMeshes;
  std::unordered_map<MeshHandle, BufferHandles> mDebugMeshes;
  MeshManager *mMeshManager;

  focus::ShaderHandle mPhongShader;
  focus::ConstantBufferHandle mPhongConstants;
  struct PhongConstants {
    glm::mat4 camera;
    glm::mat4 mvp;
    glm::mat4 normalMat;
  };

  focus::ShaderHandle mLineShader;
  focus::ConstantBufferHandle mLineConstants;

public:
  Renderer(MeshManager *meshManager) :
      mMeshManager(meshManager), 
    mPhongShader(focus::gContext->CreateShaderFromSource(
      "Phong", ReadFile("shaders/phongLight.vert"), ReadFile("shaders/phongLight.frag"))),
    mLineConstants(focus::gContext->CreateShaderFromSource(
      "Line", ReadFile("shaders/line.vert"), ReadFile("shaders/line.frag"))),
    mPhongConstants(focus::gContext->CreateConstantBuffer(nullptr, 3 * 16 * sizeof(f32), {
      .name = "constants",
      .types = {focus::VarType::Mat4, focus::VarType::Mat4, focus::VarType::Mat4}, // TODO: consider if this is necessary
      .slot = 0,
      .usage = focus::BufferUsage::Static,
      .sizeInBytes = 3 * 16 * sizeof(f32),
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
private:
};

} // namespace shared