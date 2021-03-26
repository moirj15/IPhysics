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

namespace shared
{

class Renderer
{
  using MeshHandles = std::pair<focus::VertexBufferHandle, focus::IndexBufferHandle>;
  std::unordered_map<u32, MeshHandles> mMeshes;
  MeshManager *mMeshManager;
  focus::ShaderHandle mPhongShader;
  focus::ConstantBufferHandle mPhongConstants;
  struct PhongConstants {
    glm::mat4 camera;
    glm::mat4 mvp;
    glm::mat4 normalMat;
  };

public:
  Renderer(MeshManager *meshManager) :
      mMeshManager(meshManager), 
    mPhongShader(focus::gContext->CreateShaderFromSource(
      "Phong", ReadFile("shaders/phongLight.vert"), ReadFile("shaders/phongLight.frag"))),
    mPhongConstants(focus::gContext->CreateConstantBuffer(nullptr, 3 * 16 * sizeof(f32), {
      .name = "constants",
      .types = {focus::VarType::Mat4, focus::VarType::Mat4, focus::VarType::Mat4}, // TODO: consider if this is necessary
      .slot = 0,
      .usage = focus::BufferUsage::Static,
      .sizeInBytes = 3 * 16 * sizeof(f32),
    }))
  {
  }
  void LoadMesh(MeshHandle handle);
  // TODO: stick to phong lighting for now
  // TODO: add version for textured models or other lighting models
  void DrawMesh(MeshHandle handle, const Camera &camera, const glm::mat4 &model);
  void DrawDebugVoxels(MeshHandle handle, const Camera &camera, const glm::mat4 &model);
  void ClearScreen() { focus::gContext->Clear(); }
  void UpdateScreen(const focus::Window &window) { focus::gContext->SwapBuffers(window); }
};

#if 0
using MeshHandle = u32;

enum class ShaderProgram : u32
{
  UniformColor,
  FlatLight,
  PhongLight,
  Point,
};

enum class DrawMode : u32
{
  TRIANGLES,
  LINES,
};

Window *Init(s32 width, s32 height, const char *windowName, bool enableDebug);

NODISCARD MeshHandle SubmitDynamicMesh(Mesh *mesh, ShaderProgram program);

void UpdateDynamicMesh(MeshHandle handle, const std::vector<u32> &indices, Mesh *mesh);

NODISCARD MeshHandle SubmitStaticMesh(Mesh *mesh, ShaderProgram program);

NODISCARD MeshHandle SubmitVoxelMesh(const VoxObj::VoxelMesh &voxelMesh);

void RemoveMesh(MeshHandle handle);

void Draw(MeshHandle handle, const std::vector<ShaderData> &data, DrawMode drawMode);

void SwapBuffers();
#endif

} // namespace shared