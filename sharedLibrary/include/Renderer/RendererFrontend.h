#pragma once

#include <Common.h>
#include <Utils/QuickCastBuffer.h>
#include <unordered_map>
//#include <Utils/VoxelMeshManager.h>

#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

struct Mesh;
using VMeshHandle = u32;

namespace Renderer
{

using MeshHandle = u32;

enum class ShaderProgram : u32
{
  UniformColor,
  FlatLight,
  PhongLight,
  Point,
};

void Init();

NODISCARD MeshHandle SubmitDynamicMesh(Mesh *mesh, ShaderProgram program);

void UpdateDynamicMesh(MeshHandle handle, const std::vector<u32> &indices, Mesh *mesh);

NODISCARD MeshHandle SubmitStaticMesh(Mesh *mesh, ShaderProgram program);

void RemoveMesh(MeshHandle handle);

void Draw(MeshHandle handle, const std::vector<ShaderData> &data);

void SwapBuffers();

} // namespace Renderer