#pragma once

#include "../common.h"
#include "vertexArrayObject.h"

#include <unordered_map>

struct Mesh;

using MeshPair = std::pair<VertexArray, IndexBuffer>;

class MeshLibrary
{
  std::unordered_map<u32, MeshPair> mMeshes;
  static u32 CURRENT_HANDLE;

public:
  MeshLibrary() = default;
  ~MeshLibrary();

  u32 Insert(Mesh *mesh);
  void Remove(const u32 handle);
  void Update(Mesh *mesh, const u32 handle);

  inline MeshPair operator[](const u32 i)
  {
    return mMeshes[i];
  }

private:
  MeshPair VAOFromMesh(Mesh *mesh);
};
