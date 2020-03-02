#include "Serialization.h"

#include "../voxelObjects/VoxelMesh.h"

#include <fstream>
#include <iostream>

namespace Utils
{
/*
File Structure:

int : number of voxels
int : number of mesh verts
int : number of mesh indices
int : number of mesh normals
glm::uvec3 : voxel mesh extents voxel space
glm::vec3 : voxel mesh extents object space
mesh contents
voxels
 */
void Serialize(VoxObj::VoxelMesh *voxelMesh, const std::string &path)
{
  std::ofstream file(path, std::ios::trunc | std::ios::out);
  auto *mesh = voxelMesh->GetMesh();
  file << "Vertex Count: " << mesh->mVertecies.size() << "\n";
  file << "Index Count: " << mesh->mIndecies.size() << "\n";
  file << "Normal Count: " << mesh->mNormals.size() << "\n";
  file << "Voxel Count: " << voxelMesh->GetVoxelCount() << "\n";

  const auto extentsVoxelSpace = voxelMesh->GetExtentsVoxelSpace();
  file << "Extents (Voxel Space): " << extentsVoxelSpace.x << " " << extentsVoxelSpace.y << " "
       << extentsVoxelSpace.z << "\n";

  const auto extentsObjectSpace = voxelMesh->GetExtentsObjectSpace();
  file << "Extents (Object Space): " << extentsObjectSpace.x << " " << extentsObjectSpace.y << " "
       << extentsObjectSpace.z << "\n";

  file << "Vertices\n";
  for (const auto &v : mesh->mVertecies)
  {
    file << v << " ";
  }
  file << "\nIndecies\n";
  for (const auto &i : mesh->mIndecies)
  {
    file << i << " ";
  }
  file << "\nNormals\n";
  for (const auto &n : mesh->mNormals)
  {
    file << n << " ";
  }
  file << "\nVoxels\n";
  for (const auto &[key, value] : voxelMesh->GetVoxels())
  {
    file << "Key: " << key.x << " " << key.y << " " << key.z << "\n";
    file << "Dimensions: " << value.mDimensions.x << " " << value.mDimensions.y << " "
         << value.mDimensions.z << "\n";
    file << "Neighbor Count: " << value.mNeighbors.size() << "\n";
    for (const auto &n : value.mNeighbors)
    {
      file << n.x << " " << n.y << " " << n.z << "\n";
    }
    file << "Contained Mesh Vertex Count: " << value.mMeshVertices.size() << "\n";
    for (const auto &v : value.mMeshVertices)
    {
      file << v << " ";
    }
    file << "\n";
  }
}

VoxObj::VoxelMesh *DeSerialize(const std::string &path)
{
  std::fstream file(path, std::ios::trunc | std::ios::in);
  u64 vertCount = 0;
  u64 indexCount = 0;
  u64 normalCount = 0;
  u64 voxelCount = 0;

  file >> vertCount;
  file >> indexCount;
  file >> normalCount;
  file >> voxelCount;

  glm::uvec3 extentsVoxelSpace(0, 0, 0);
  file >> extentsVoxelSpace.x >> extentsVoxelSpace.y >> extentsVoxelSpace.z;

  glm::vec3 extentsObjectSpace(0.0f);
  file >> extentsObjectSpace.x >> extentsObjectSpace.y >> extentsObjectSpace.z;

  Mesh *mesh = new Mesh;
  mesh->mVertecies.reserve(vertCount);
  mesh->mIndecies.reserve(indexCount);
  mesh->mNormals.reserve(normalCount);

  auto vm = new VoxObj::VoxelMesh(extentsVoxelSpace, extentsObjectSpace, mesh);

  for (u64 i = 0; i < vertCount; i++)
  {
    file >> mesh->mVertecies[i];
  }
  for (u64 i = 0; i < indexCount; i++)
  {
    file >> mesh->mIndecies[i];
  }
  for (u64 i = 0; i < normalCount; i++)
  {
    file >> mesh->mNormals[i];
  }
  for (u64 i = 0; i < voxelCount; i++)
  {
    VoxObj::Voxel voxel;
    glm::uvec3 key;
    file >> key.x >> key.y >> key.z;
    file >> voxel.mDimensions.x >> voxel.mDimensions.y >> voxel.mDimensions.z;
    u64 neighborCount = 0;
    file >> neighborCount;
    voxel.mNeighbors.reserve(neighborCount / 3);
    for (u64 n = 0; n < neighborCount; n += 3)
    {
      u64 index = n / 3;
      file >> voxel.mNeighbors[index].x >> voxel.mNeighbors[index].y >> voxel.mNeighbors[index].z;
    }
    u64 containedVertCount = 0;
    file >> containedVertCount;
    voxel.mMeshVertices.reserve(containedVertCount);
    for (u64 v = 0; v < containedVertCount; v++)
    {
      file >> voxel.mMeshVertices[v];
    }
    vm->SetVoxel(key, voxel);
  }
  return vm;
}
} // namespace Utils