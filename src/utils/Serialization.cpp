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
  file << "[Vertex Count]\n" << mesh->mVertecies.size() << "\n";
  file << "[Index Count]\n" << mesh->mIndecies.size() << "\n";
  file << "[Normal Count]\n" << mesh->mNormals.size() << "\n";
  file << "[Voxel Count]\n" << voxelMesh->GetVoxelCount() << "\n";

  const auto extentsVoxelSpace = voxelMesh->GetExtentsVoxelSpace();
  file << "[Extents (Voxel Space)]\n"
       << extentsVoxelSpace.x << " " << extentsVoxelSpace.y << " " << extentsVoxelSpace.z << "\n";

  const auto extentsObjectSpace = voxelMesh->GetExtentsObjectSpace();
  file << "[Extents (Object Space)]\n"
       << extentsObjectSpace.x << " " << extentsObjectSpace.y << " " << extentsObjectSpace.z
       << "\n";

  file << "[Vertices]\n";
  for (const auto &v : mesh->mVertecies)
  {
    file << v << " ";
  }
  file << "\n[Indecies]\n";
  for (const auto &i : mesh->mIndecies)
  {
    file << i << " ";
  }
  file << "\n[Normals]\n";
  for (const auto &n : mesh->mNormals)
  {
    file << n << " ";
  }
  file << "\n[Voxels]\n";
  for (const auto &[key, value] : voxelMesh->GetVoxels())
  {
    file << "[Key]\n" << key.x << " " << key.y << " " << key.z << "\n";
    file << "[Dimensions]\n"
         << value.mDimensions.x << " " << value.mDimensions.y << " " << value.mDimensions.z << "\n";
    file << "[Neighbor Count]\n" << value.mNeighbors.size() << "\n";
    for (const auto &n : value.mNeighbors)
    {
      file << n.x << " " << n.y << " " << n.z << "\n";
    }
    file << "[Contained Mesh Vertex Count]\n" << value.mMeshVertices.size() << "\n";
    for (const auto &v : value.mMeshVertices)
    {
      file << v << " ";
    }
    // file << "\n";
  }
}

VoxObj::VoxelMesh *DeSerialize(const std::string &path)
{
  std::fstream file(path, std::ios::in);
  std::cout << file.good() << '\n';
  u64 vertCount = 0;
  u64 indexCount = 0;
  u64 normalCount = 0;
  u64 voxelCount = 0;

  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> vertCount;
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> indexCount;
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> normalCount;
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> voxelCount;

  glm::uvec3 extentsVoxelSpace(0, 0, 0);

  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> extentsVoxelSpace.x >> extentsVoxelSpace.y >> extentsVoxelSpace.z;

  glm::vec3 extentsObjectSpace(0.0f);
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file >> extentsObjectSpace.x >> extentsObjectSpace.y >> extentsObjectSpace.z;

  Mesh *mesh = new Mesh;
  mesh->mVertecies.resize(vertCount);
  mesh->mIndecies.resize(indexCount);
  mesh->mNormals.resize(normalCount);

  auto vm = new VoxObj::VoxelMesh(extentsVoxelSpace, extentsObjectSpace, mesh);

  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (u64 i = 0; i < vertCount; i++)
  {
    file >> mesh->mVertecies[i];
  }
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (u64 i = 0; i < indexCount; i++)
  {
    file >> mesh->mIndecies[i];
  }
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (u64 i = 0; i < normalCount; i++)
  {
    file >> mesh->mNormals[i];
  }
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (u64 i = 0; i < voxelCount; i++)
  {
    VoxObj::Voxel voxel;
    glm::uvec3 key;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file >> key.x >> key.y >> key.z;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file >> voxel.mDimensions.x >> voxel.mDimensions.y >> voxel.mDimensions.z;
    u64 neighborCount = 0;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file >> neighborCount;
    voxel.mNeighbors.reserve(neighborCount / 3);
    for (u64 n = 0; n < neighborCount; n += 3)
    {
      u64 index = n / 3;
      file >> voxel.mNeighbors[index].x >> voxel.mNeighbors[index].y >> voxel.mNeighbors[index].z;
    }
    u64 containedVertCount = 0;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file >> containedVertCount;
    voxel.mMeshVertices.resize(containedVertCount);
    for (u64 v = 0; v < containedVertCount; v++)
    {
      file >> voxel.mMeshVertices[v];
    }
    vm->SetVoxel(key, voxel);
  }
  return vm;
}
} // namespace Utils