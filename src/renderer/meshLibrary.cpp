#include "meshLibrary.h"

#include "mesh.h"

u32 MeshLibrary::CURRENT_HANDLE = 0;

MeshLibrary::~MeshLibrary()
{
  for (auto &pair : mMeshes)
  {
    auto [vbo, ibo] = pair.second;
    vbo.Destroy();
    ibo.Destroy();
  }
}

u32 MeshLibrary::Insert(Mesh *mesh)
{
  CURRENT_HANDLE++;
  auto [vao, ibo] = VAOFromMesh(mesh);
  mMeshes.emplace(CURRENT_HANDLE, std::make_pair(vao, ibo));
  return CURRENT_HANDLE;
}

void MeshLibrary::Remove(const u32 handle)
{
  if (handle == 0)
  {
    return;
  }
  auto [vao, ibo] = mMeshes[handle];
  vao.Destroy();
  ibo.Destroy();
  mMeshes.erase(handle);
}

void MeshLibrary::Update(Mesh *mesh, const u32 handle)
{
  // TODO: there's probably a better way to do this without constantly reconstructing and sending
  // the data to the gpu.
  auto [vao, ibo] = VAOFromMesh(mesh);
  auto [removedVAO, removedIBO] = mMeshes[handle];
  removedVAO.Destroy();
  removedIBO.Destroy();
  mMeshes[handle] = std::make_pair(vao, ibo);
}

MeshPair MeshLibrary::VAOFromMesh(Mesh *mesh)
{
  VertexArray vao;
  vao.Create();
  IndexBuffer ibo;
  ibo.Create(mesh->mIndices.data(), mesh->mIndices.size());
  vao.AddIBO(ibo);
  if (!mesh->mVertices.IsEmpty())
  {
    vao.AddVBO(VertexBuffer(
        mesh->mVertices.BufferPtr(), mesh->mVertices.BufferSize(),
        BufferLayout("Vertexes", 3, 0, 0, GL_FLOAT)));
  }
  if (!mesh->mNormals.IsEmpty())
  {
    vao.AddVBO(VertexBuffer(
        mesh->mNormals.BufferPtr(), mesh->mNormals.BufferSize(),
        BufferLayout("Normals", 3, 0, 1, GL_FLOAT)));
  }
  if (!mesh->mVertices.IsEmpty())
  {
    vao.AddVBO(VertexBuffer(
        mesh->mTextureCoords.BufferPtr(), mesh->mTextureCoords.BufferSize(),
        BufferLayout("UVs", 2, 0, 2, GL_FLOAT)));
  }
  return {vao, ibo};
}
