#include "voxelizer.h"

#include "../renderer/mesh.h"

namespace VoxGen
{
VoxObj::VoxelMesh Voxelizer::Voxelize(Mesh *mesh)
{
  auto meshAABB = FindMeshAABB(mesh);
  auto triangleAABBs = FindTriangleAABBs(mesh);
  auto voxelMesh = GenerateVoxels(triangleAABBs, meshAABB, mesh);
  if (!mParameters.mHollow)
  {
    FillVoxelMesh(&voxelMesh);
  }

  return voxelMesh;
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
    const std::vector<std::array<rp3d::Vector3, 3>> &meshTriangles, const rp3d::AABB &meshAABB,
    Mesh *mesh)
{
  glm::vec3 aabbExtents(
      glm::ceil(glm::vec3(meshAABB.getExtent().x, meshAABB.getExtent().y, meshAABB.getExtent().z)));
  // Calculate the extents of the voxel mesh in discrete voxel space
  glm::uvec3 voxelMeshExtents(aabbExtents / mParameters.mVoxelSize);
  VoxObj::VoxelMesh voxelMesh(
      voxelMeshExtents, aabbExtents, glm::vec3(mParameters.mVoxelSize), mesh);
  const auto &minCoords = meshAABB.getMin();
  for (u32 x = 0; x < voxelMeshExtents.x; x++)
  {
    for (u32 y = 0; y < voxelMeshExtents.y; y++)
    {
      for (u32 z = 0; z < voxelMeshExtents.z; z++)
      {
        // Calculate the offset used to create the AABB used to represent the voxel
        rp3d::Vector3 offset(
            f32(x) * mParameters.mVoxelSize, f32(y) * mParameters.mVoxelSize,
            f32(z) * mParameters.mVoxelSize);
        rp3d::AABB voxel(
            minCoords + offset,
            minCoords
                + (offset
                   + rp3d::Vector3(
                       mParameters.mVoxelSize, mParameters.mVoxelSize, mParameters.mVoxelSize)));
        // Find the triangles that the voxel intersects
        for (const auto &aabb : meshTriangles)
        {
          if (voxel.testCollisionTriangleAABB(aabb.data()))
          {
            glm::ivec3 position(x, y, z);
            // TODO: add voxel position
            voxelMesh.SetVoxel(position, VoxObj::Voxel(mParameters.mVoxelSize, {}));
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

void Voxelizer::FillVoxelMesh(VoxObj::VoxelMesh *voxelMesh)
{
  std::vector<std::pair<glm::uvec3, glm::uvec3>> fillList;
  const auto &extents = voxelMesh->GetExtentsVoxelSpace();
  for (u32 z = 0; z < extents.z; z++)
  {
    for (u32 y = 0; y < extents.y; y++)
    {
      glm::uvec3 start(0, 0, z);
      glm::uvec3 end(0, 0, z);
      for (u32 x = 0; x < extents.x; x++)
      {
        glm::uvec3 position(x, y, z);
        glm::uvec3 prevPosition(x - 1, y, z);
        // Check if this position is the start of a gap that needs to be filled
        if (voxelMesh->IsVoxelPresent(prevPosition) && !voxelMesh->IsVoxelPresent(position))
        {
          start = position;
        }
        // Check if this position is the end of a gap that needs to be filled
        else if (!voxelMesh->IsVoxelPresent(prevPosition) && voxelMesh->IsVoxelPresent(position))
        {
          end = position;
          fillList.emplace_back(start, end);
        }
      }
    }
  }
  // Use the fill list to find the holes that need to be filled and fill them with new voxels.
  for (const auto &[start, end] : fillList)
  {
    for (u32 x = start.x; x < end.x; x++)
    {
      // TODO: should also update the voxel neighbors.
      voxelMesh->SetVoxel(glm::uvec3(x, start.y, start.z), VoxObj::Voxel());
    }
  }
}
} // namespace VoxGen
