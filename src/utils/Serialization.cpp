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
  file << mesh->mVertecies.size() << "\n";
  file << mesh->mIndecies.size() << "\n";
  file << mesh->mNormals.size() << "\n";

  const auto extentsVoxelSpace = voxelMesh->GetExtentsVoxelSpace();
  file << extentsVoxelSpace.x << " " << extentsVoxelSpace.y << " " << extentsVoxelSpace.z << "\n";

  const auto extentsObjectSpace = voxelMesh->GetExtentsObjectSpace();
  file << extentsObjectSpace.x << " " << extentsObjectSpace.y << " " << extentsObjectSpace.z
       << "\n";

  for (const auto &v : mesh->mVertecies)
  {
    file << v << " ";
  }
  file << "\n";
  for (const auto &i : mesh->mIndecies)
  {
    file << i << " ";
  }
  file << "\n";
  for (const auto &n : mesh->mNormals)
  {
    file << n << " ";
  }
  file << "\n";
  for (const auto &[key, value] : voxelMesh->GetVoxels())
  {
    file << key.x << " " << key.y << " " << key.z << " ";
    file << value.mDimensions.x << " " << value.mDimensions.y << " " << value.mDimensions.z << " ";
    for (const auto &n : value.mNeighbors)
    {
      file << n.x << " " << n.y << " " << n.z << " ";
    }
    for (const auto &v : value.mMeshVerteciess)
    {
      file << v << " ";
    }
    file << "\n";
  }
}

VoxObj::VoxelMesh *DeSerialize(const std::string &path)
{
  return nullptr;
}
} // namespace Utils