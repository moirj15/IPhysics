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

// std::vector<rp3d::AABB> Voxelizer::FindTriangleAABBs(Mesh *mesh)
// {
//   std::vector<rp3d::AABB> AABBs;
//   for (u64 i = 0; i < mesh->mIndecies.size(); i += 3)
//   {
//
//     rp3d::Vector3 points[3] = {rp3d::Vector3(
//                                    mesh->mVertecies[(mesh->mIndecies[i] * 3)],
//                                    mesh->mVertecies[(mesh->mIndecies[i] * 3) + 1],
//                                    mesh->mVertecies[(mesh->mIndecies[i] * 3) + 2]),
//                                rp3d::Vector3(
//                                    mesh->mVertecies[(mesh->mIndecies[(i + 1)] * 3)],
//                                    mesh->mVertecies[(mesh->mIndecies[(i + 1)] * 3) + 1],
//                                    mesh->mVertecies[(mesh->mIndecies[(i + 1)] * 3) + 2]),
//                                rp3d::Vector3(
//                                    mesh->mVertecies[(mesh->mIndecies[(i + 2)] * 3)],
//                                    mesh->mVertecies[(mesh->mIndecies[(i + 2)] * 3) + 1],
//                                    mesh->mVertecies[(mesh->mIndecies[(i + 2)] * 3) + 2])};
//     rp3d::Vector3 min = points[0];
//     rp3d::Vector3 max = points[1];
//     for (u64 j = 0; j < 3; j++)
//     {
//       min = rp3d::Vector3::min(points[j], points[(j + 1) % 3]);
//       max = rp3d::Vector3::max(points[j], points[(j + 1) % 3]);
//     }
//     AABBs.emplace_back(min, max);
//   }
//   return AABBs;
// }
std::vector<std::array<rp3d::Vector3, 3>> Voxelizer::FindTriangleAABBs(Mesh *mesh)
{
  std::vector<std::array<rp3d::Vector3, 3>> AABBs;
  for (u64 i = 0; i < mesh->mIndecies.size(); i += 3)
  {
    AABBs.push_back({{rp3d::Vector3(
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
                          mesh->mVertecies[(mesh->mIndecies[(i + 2)] * 3) + 2])}});
  }
  return AABBs;
}

VoxObj::VoxelMesh Voxelizer::GenerateVoxels(
    const std::vector<std::array<rp3d::Vector3, 3>> &triangleAABBs, const rp3d::AABB &meshAABB,
    Mesh *mesh)
{
  glm::vec3 aabbExtents(
      glm::ceil(glm::vec3(meshAABB.getExtent().x, meshAABB.getExtent().y, meshAABB.getExtent().z)));
  glm::ivec3 voxelMeshExtents(aabbExtents / mParameters.mVoxelSize);
  VoxObj::VoxelMesh voxelMesh(voxelMeshExtents, aabbExtents, mesh);
  const auto &minCoords = meshAABB.getMin();
  for (u32 x = 0; x < voxelMeshExtents.x; x++)
  {
    for (u32 y = 0; y < voxelMeshExtents.y; y++)
    {
      for (u32 z = 0; z < voxelMeshExtents.z; z++)
      {
        // TODO: there's something wrong with this offset, cuases the (1, 1, 1) voxel to have
        // min=max
        rp3d::Vector3 offset(
            f32(x) * mParameters.mVoxelSize, f32(y) * mParameters.mVoxelSize,
            f32(z) * mParameters.mVoxelSize);
        rp3d::AABB voxel(
            minCoords + offset,
            rp3d::Vector3(mParameters.mVoxelSize, mParameters.mVoxelSize, mParameters.mVoxelSize)
                + minCoords);
        for (const auto &aabb : triangleAABBs)
        {
          if (voxel.testCollisionTriangleAABB(aabb.data()))
          {
            glm::ivec3 position(x, y, z);
            voxelMesh.SetVoxel(position, VoxObj::Voxel(mParameters.mVoxelSize));
            // TODO: probably a better idea to add the point, edge, or triangle that the voxel
            // contains here
            break;
          }
        }
      }
    }
  }
  return voxelMesh;
}

} // namespace VoxGen
