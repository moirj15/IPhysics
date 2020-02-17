#include "voxelizer.h"

#include "../renderer/mesh.h"

namespace VoxGen
{
std::vector<VoxObj::Voxel> Voxelizer::Voxelize(Mesh *mesh)
{
  auto meshAABB = FindMeshAABB(mesh);
  auto triangleAABBs = FindTriangleAABBs(mesh);
  auto voxelMesh = GenerateVoxels(triangleAABBs, meshAABB, mesh);

  return {};
}

// Private
rp3d::AABB Voxelizer::FindMeshAABB(Mesh *mesh)
{
  rp3d::Vector3 min(0.0f, 0.0f, 0.0f);
  rp3d::Vector3 max(0.0f, 0.0f, 0.0f);
  for (u64 i = 0; i < mesh->mIndecies.size(); i++)
  {
    const f32 x = mesh->mVertecies[(mesh->mIndecies[i]) * 3];
    const f32 y = mesh->mVertecies[(mesh->mIndecies[i] * 3) + 1];
    const f32 z = mesh->mVertecies[(mesh->mIndecies[i] * 3) + 2];
    rp3d::Vector3 currentPoint(x, y, z);
    min = rp3d::Vector3::min(currentPoint, min);
    max = rp3d::Vector3::max(currentPoint, max);
  }
  return rp3d::AABB(min, max);
}

std::vector<rp3d::AABB> Voxelizer::FindTriangleAABBs(Mesh *mesh)
{
  std::vector<rp3d::AABB> AABBs;
  for (u64 i = 0; i < mesh->mIndecies.size(); i += 3)
  {

    rp3d::Vector3 points[3] = {rp3d::Vector3(
                                   mesh->mVertecies[(mesh->mIndecies[i] * 3)],
                                   mesh->mVertecies[(mesh->mIndecies[i] * 3) + 1],
                                   mesh->mVertecies[(mesh->mIndecies[i] * 3) + 2]),
                               rp3d::Vector3(
                                   mesh->mVertecies[(mesh->mIndecies[(i + 1)] * 3)],
                                   mesh->mVertecies[(mesh->mIndecies[(i + 1)] * 3) + 1],
                                   mesh->mVertecies[(mesh->mIndecies[(i + 1)] * 3) + 2]),
                               rp3d::Vector3(
                                   mesh->mVertecies[(mesh->mIndecies[(i + 2)] * 3)],
                                   mesh->mVertecies[(mesh->mIndecies[(i + 2)] * 3) + 1],
                                   mesh->mVertecies[(mesh->mIndecies[(i + 2)] * 3) + 2])};
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

VoxObj::VoxelMesh Voxelizer::GenerateVoxels(
    const std::vector<rp3d::AABB> &triangleAABBs, const rp3d::AABB &meshAABB, Mesh *mesh)
{
  glm::vec3 aabbExtents(meshAABB.getExtent().x, meshAABB.getExtent().y, meshAABB.getExtent().z);
  glm::ivec3 voxelMeshExtents(glm::ceil(aabbExtents / mParameters.mVoxelSize));
  VoxObj::VoxelMesh voxelMesh(voxelMeshExtents, mesh);
  auto minCoords = meshAABB.getMin();
  for (f32 x = 0; x < (f32)voxelMeshExtents.x; x += mParameters.mVoxelSize)
  {
    for (f32 y = 0; y < (f32)voxelMeshExtents.y; y += mParameters.mVoxelSize)
    {
      for (f32 z = 0; z < (f32)voxelMeshExtents.z; z += mParameters.mVoxelSize)
      {
        rp3d::Vector3 offset(x, y, z);
        rp3d::AABB voxel(
            minCoords + offset,
            rp3d::Vector3(mParameters.mVoxelSize, mParameters.mVoxelSize, mParameters.mVoxelSize));
        for (const auto &aabb : triangleAABBs)
        {
          if (voxel.testCollision(aabb))
          {

            glm::ivec3 position(
                u32(x / mParameters.mVoxelSize), u32(y / mParameters.mVoxelSize),
                u32(z / mParameters.mVoxelSize));
            voxelMesh.SetVoxel(position, VoxObj::Voxel(mParameters.mVoxelSize));
          }
        }
      }
    }
  }
  return voxelMesh;
}

} // namespace VoxGen
