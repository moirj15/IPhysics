#pragma once

#include "../common.h"
#include "vertexArrayObject.h"

#include <unordered_map>

struct Mesh;

class MeshLibrary
{
  std::unordered_map<u32, VertexArray> mMeshes;
  static u32 CURRENT_HANDLE;

public:
  MeshLibrary() = default;
  ~MeshLibrary();

  u32 Insert(Mesh *mesh);
  void Remove(const u32 handle);
  void Update(Mesh *mesh, const u32 handle);

private:
  VertexArray VAOFromMesh(Mesh *mesh);
};
