#include "voxelizer.h"

#include "../renderer/mesh.h"

namespace VoxGen
{
std::vector<vx::Voxel> Voxelizer::Voxelize(Mesh *mesh, f32 size, bool isHollow)
{
  auto meshAABB = FindMeshAABB(mesh);
  auto triangleAABBs = FindTriangleAABBs(mesh);
  return {};
}

rp3d::AABB Voxelizer::FindMeshAABB(Mesh *mesh)
{
  rp3d::Vector3 min(0.0f, 0.0f, 0.0f);
  rp3d::Vector3 max(0.0f, 0.0f, 0.0f);
  for (u64 i = 0; i < mesh->mIndecies.size(); i += 3)
  {
    const f32 x = mesh->mVertecies[mesh->mIndecies[i]];
    const f32 y = mesh->mVertecies[mesh->mIndecies[i + 1]];
    const f32 z = mesh->mVertecies[mesh->mIndecies[i + 2]];
    rp3d::Vector3 currentPoint(x, y, z);
    min = rp3d::Vector3::min(currentPoint, min);
    max = rp3d::Vector3::max(currentPoint, max);
  }
  return rp3d::AABB(min, max);
}

std::vector<rp3d::AABB> Voxelizer::FindTriangleAABBs(Mesh *mesh)
{
  std::vector<rp3d::AABB> AABBs;
  for (u64 i = 0; i < mesh->mIndecies.size(); i += 9)
  {
    rp3d::Vector3 points[3] = {rp3d::Vector3(
                                   mesh->mVertecies[mesh->mIndecies[i]],
                                   mesh->mVertecies[mesh->mIndecies[i + 1]],
                                   mesh->mVertecies[mesh->mIndecies[i + 2]]),
                               rp3d::Vector3(
                                   mesh->mVertecies[mesh->mIndecies[(i * 2)]],
                                   mesh->mVertecies[mesh->mIndecies[(i * 2) + 1]],
                                   mesh->mVertecies[mesh->mIndecies[(i * 2) + 2]]),
                               rp3d::Vector3(
                                   mesh->mVertecies[mesh->mIndecies[i * 3]],
                                   mesh->mVertecies[mesh->mIndecies[(i * 3) + 1]],
                                   mesh->mVertecies[mesh->mIndecies[(i * 3) + 2]])};
    rp3d::Vector3 min;
    rp3d::Vector3 max;
    for (u64 j = 0; j < 3; j++)
    {
      min = rp3d::Vector3::min(points[i], points[i % 3]);
      max = rp3d::Vector3::max(points[i], points[i % 3]);
    }
    AABBs.emplace_back(min, max);
  }
  return AABBs;
}
} // namespace VoxGen
