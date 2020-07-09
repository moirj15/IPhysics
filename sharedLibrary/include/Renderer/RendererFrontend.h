#pragma once

#include "ShaderData.h"

#include <Common.h>
#include <Utils/QuickCastBuffer.h>
#include <unordered_map>
//#include <Utils/VoxelMeshManager.h>

#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

struct Mesh;
using VMeshHandle = u32;

struct Window;
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

enum class DrawMode : u32
{
  TRIANGLES,
  LINES,
};

Window *Init(s32 width, s32 height, const char *windowName, bool enableDebug);

NODISCARD MeshHandle SubmitDynamicMesh(Mesh *mesh, ShaderProgram program);

void UpdateDynamicMesh(MeshHandle handle, const std::vector<u32> &indices, Mesh *mesh);

NODISCARD MeshHandle SubmitStaticMesh(Mesh *mesh, ShaderProgram program);

void RemoveMesh(MeshHandle handle);

void Draw(MeshHandle handle, const std::vector<ShaderData> &data, DrawMode drawMode);

void SwapBuffers();

} // namespace Renderer