#include "voxelizer.h"

#include "../PhysicsEngine/RayCastWorld.h"
#include "../renderer/mesh.h"
#include "../voxelObjects/EdgeMesh.h"

#include <unordered_set>

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
  AddNeighbors(&voxelMesh);

  return voxelMesh;
}

// Private
btAABB Voxelizer::FindMeshAABB(Mesh *mesh)
{
  btVector3 min(0.0f, 0.0f, 0.0f);
  btVector3 max(0.0f, 0.0f, 0.0f);
  for (u64 i = 0; i < mesh->mIndices.size(); i++)
  {
    auto v = mesh->mVertices.AccessCastBuffer(mesh->mIndices[i]);
    //     const f32 x = mesh->mVertices[(mesh->mIndices[i]) * 3];
    //     const f32 y = mesh->mVertices[(mesh->mIndices[i] * 3) + 1];
    //     const f32 z = mesh->mVertices[(mesh->mIndices[i] * 3) + 2];
    btVector3 currentPoint(v.x, v.y, v.z);
    min.setMin(currentPoint);
    max.setMax(currentPoint);
  }
  return btAABB(min, max, min);
}

std::vector<Voxelizer::MeshInfo> Voxelizer::FindTriangleAABBs(Mesh *mesh)
{
  std::vector<MeshInfo> meshInfos;
  for (u64 i = 0; i < mesh->mIndices.size(); i += 3)
  {
    auto v0 = mesh->mVertices.AccessCastBuffer(mesh->mIndices[i]);
    auto v1 = mesh->mVertices.AccessCastBuffer(mesh->mIndices[i + 1]);
    auto v2 = mesh->mVertices.AccessCastBuffer(mesh->mIndices[i + 2]);
    btVector3 points[] = {btVector3(v0.x, v0.y, v0.z), btVector3(v1.x, v1.y, v1.z),
                          btVector3(v2.x, v2.y, v2.z)};
    //     btVector3 points[] = {btVector3(
    //                               mesh->mVertices[(mesh->mIndices[i] * 3)],
    //                               mesh->mVertices[(mesh->mIndices[i] * 3) + 1],
    //                               mesh->mVertices[(mesh->mIndices[i] * 3) + 2]),
    //                           btVector3(
    //                               mesh->mVertices[(mesh->mIndices[(i + 1)] * 3)],
    //                               mesh->mVertices[(mesh->mIndices[(i + 1)] * 3) + 1],
    //                               mesh->mVertices[(mesh->mIndices[(i + 1)] * 3) + 2]),
    //                           btVector3(
    //                               mesh->mVertices[(mesh->mIndices[(i + 2)] * 3)],
    //                               mesh->mVertices[(mesh->mIndices[(i + 2)] * 3) + 1],
    //                               mesh->mVertices[(mesh->mIndices[(i + 2)] * 3) + 2])};
    u32 indices[] = {mesh->mIndices[i], mesh->mIndices[i + 1], mesh->mIndices[i + 2]};
    meshInfos.push_back(MeshInfo(points, indices));
    // Add the indices to the points
  }
  return meshInfos;
}

VoxObj::VoxelMesh
Voxelizer::GenerateVoxels(std::vector<MeshInfo> &meshTriangles, const btAABB &meshAABB, Mesh *mesh)
{
  glm::vec3 min(meshAABB.m_min.x(), meshAABB.m_min.y(), meshAABB.m_min.z());
  glm::vec3 max(meshAABB.m_max.x(), meshAABB.m_max.y(), meshAABB.m_max.z());
  glm::vec3 extentsOffset(-meshAABB.m_min.x(), -meshAABB.m_min.y(), -meshAABB.m_min.z());
  // move the min and max to be centered at the origin
  max += extentsOffset;

  // use the max of the AABB as the extents
  glm::vec3 aabbExtents(glm::ceil(max));
  // Calculate the extents of the voxel mesh in discrete voxel space
  glm::uvec3 voxelMeshExtents(aabbExtents / mParameters.mVoxelSize);
  VoxObj::VoxelMesh voxelMesh(
      voxelMeshExtents, aabbExtents, glm::vec3(mParameters.mVoxelSize), mesh);
  for (u32 x = 0; x < voxelMeshExtents.x; x++)
  {
    for (u32 y = 0; y < voxelMeshExtents.y; y++)
    {
      for (u32 z = 0; z < voxelMeshExtents.z; z++)
      {
        // Calculate the offset used to create the AABB used to represent the voxel
        btVector3 offset(
            f32(x) * mParameters.mVoxelSize, f32(y) * mParameters.mVoxelSize,
            f32(z) * mParameters.mVoxelSize);
        btVector3 btMin(min.x, min.y, min.z);
        btAABB voxel(
            btMin + offset,
            btMin
                + (offset
                   + btVector3(
                       mParameters.mVoxelSize, mParameters.mVoxelSize, mParameters.mVoxelSize)),
            btMin + offset, mParameters.mVoxelSize / 100.0f);
        // Find the triangles that the voxel intersects
        glm::ivec3 key(x, y, z);
        auto minToMaxVec = voxel.m_max - voxel.m_min;
        auto length = minToMaxVec.length();
        btVector3 voxelCenter(voxel.m_min + ((voxel.m_max - voxel.m_min) / 2.0f));
        glm::vec3 position(voxelCenter.x(), voxelCenter.y(), voxelCenter.z());
        VoxObj::Voxel generatedVoxel(mParameters.mVoxelSize, position);
        std::unordered_set<u32> voxelVertices;
        bool keep = false;
        for (auto &triangle : meshTriangles)
        {
          btAABB triangleAABB(triangle.mPoints[0], triangle.mPoints[1], triangle.mPoints[2]);
          if (voxel.has_collision(triangleAABB))
          // if (testBox.testCollisionTriangleAABB(triangle.first.data()))
          {
            keep = true;
            for (u32 i = 0; i < ArraySize(triangle.mPoints); i++)
            {
              if ((voxel.m_min.x() <= triangle.mPoints[i].x())
                  && (voxel.m_min.y() <= triangle.mPoints[i].y())
                  && (voxel.m_min.z() <= triangle.mPoints[i].z())
                  && (triangle.mPoints[i].x() <= voxel.m_max.x())
                  && (triangle.mPoints[i].y() <= voxel.m_max.y())
                  && (triangle.mPoints[i].z() <= voxel.m_max.z()) && (!triangle.mInVoxel[i]))
              // if (voxel.contains(triangle.first[i]))
              // if (testBox.contains(triangle.first[i]))
              {
                //                 generatedVoxel.mMeshVertices.push_back(triangle.mIndices[i]);
                voxelVertices.emplace(triangle.mIndices[i]);
                triangle.mInVoxel[i] = true;
              }
            }
          }
        }
        if (keep)
        {
          for (u32 v : voxelVertices)
          {
            generatedVoxel.mMeshVertices.emplace_back(v);
          }
          voxelMesh.SetVoxel(key, generatedVoxel);
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

void Voxelizer::AddNeighbors(VoxObj::VoxelMesh *voxelMesh)
{
  for (auto &[key, voxel] : voxelMesh->mVoxels)
  {
    glm::uvec3 keys[] = {
        glm::uvec3(key.x - 1, key.y, key.z), glm::uvec3(key.x + 1, key.y, key.z),

        glm::uvec3(key.x, key.y - 1, key.z), glm::uvec3(key.x, key.y + 1, key.z),

        glm::uvec3(key.x, key.y, key.z - 1), glm::uvec3(key.x, key.y, key.z + 1),
    };
    for (const auto &k : keys)
    {
      if (voxelMesh->mVoxels.find(k) != voxelMesh->mVoxels.end())
      {
        voxel.mNeighbors.emplace_back(k);
      }
    }
  }
}

void Voxelizer::AddBezierCurves(VoxObj::VoxelMesh *voxelMesh)
{
  auto edgeMap = CreateEdgeMap(voxelMesh);
  RayCastWorld rayCastWorld;
  std::vector<Box> boxes;
  for (const auto &[_, voxel] : voxelMesh->mVoxels)
  {
    boxes.emplace_back(voxel.mPosition, voxel.mDimensions);
  }
  rayCastWorld.AddBoxes(boxes);
  for (const auto &[_, voxel] : voxelMesh->mVoxels)
  {
    for (u32 index : voxel.mMeshVertices)
    {
      auto edge = edgeMap[index];
      //       auto intersections = rayCastWorld.CastRay(Ray())
    }
  }
}

std::unordered_map<u32, Edge> Voxelizer::CreateEdgeMap(VoxObj::VoxelMesh *voxelMesh)
{
  std::unordered_map<u32, Edge> edgeMap;
  auto &AddEdgeNoDuplicates = [&edgeMap](const u32 v0, const u32 v1) {
    if (edgeMap.find(v0) != edgeMap.end())
    {
      edgeMap.emplace(v1, Edge(v1, v0));
    }
    else
    {
      edgeMap.emplace(v0, Edge(v0, v1));
    }
  };
  for (u64 i = 0; i < voxelMesh->mMesh->mIndices.size(); i += 3)
  {
    u32 v0 = voxelMesh->mMesh->mIndices[i];
    u32 v1 = voxelMesh->mMesh->mIndices[i + 1];
    u32 v2 = voxelMesh->mMesh->mIndices[i + 2];

    AddEdgeNoDuplicates(v0, v1);
    AddEdgeNoDuplicates(v1, v2);
    AddEdgeNoDuplicates(v2, v0);
  }
  return edgeMap;
}

} // namespace VoxGen
