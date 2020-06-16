#pragma once

#include "../../Common.h"
#include "VertexArrayObject.h"

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

  /**
   * \brief: Insert a mesh into the MeshLibrary.
   * \param mesh: The mesh that will be inserted.
   * \return: A handle that can be used to retrieve the mesh later.
   */
  u32 Insert(Mesh *mesh);
  /**
   * \brief: Remove the mesh from the library with the given handle.
   * \param handle: The handle of the mesh that we want to remove.
   */
  void Remove(const u32 handle);
  /**
   * \brief: Update the contents of the mesh library with the given mesh and handle.
   * \param mesh: The mesh that will be used to update the contents of the mesh library.
   * \param handle: The handle of the mesh that we're updating.
   */
  void Update(Mesh *mesh, const u32 handle);

  inline MeshPair operator[](const u32 i)
  {
    return mMeshes[i];
  }

private:
  /**
   * \brief: Converts the given mesh to a VertexArray Object.
   * \param mesh: The mesh that will be converted to a VertexArray Object.
   * \return: A pair containing the VertexArray and IndexBuffer.
   */
  MeshPair VAOFromMesh(Mesh *mesh);
};
