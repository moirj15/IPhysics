#pragma once

#include "../common.h"
#include "../voxelObjects/VoxelMesh.h"
#include "../voxelObjects/voxel.h"
#include "VoxelizerParameters.h"

#include <BulletCollision/Gimpact/btBoxCollision.h>
#include <array>
#include <vector>

struct Mesh;

namespace VoxGen
{

class Voxelizer
{
  Parameters mParameters;

public:
  NODISCARD VoxObj::VoxelMesh Voxelize(Mesh *mesh);

  inline void SetParameters(const Parameters &parameters)
  {
    mParameters = parameters;
  }
  inline void Reset()
  {
    mParameters = Parameters();
  }

private:
  struct MeshInfo
  {
    btVector3 mPoints[3];
    u32 mIndices[3];
    bool mInVoxel[3];
    MeshInfo(btVector3 points[3], u32 indices[3]) :
        mPoints{points[0], points[1], points[2]}, mIndices{indices[0], indices[1], indices[2]},
        mInVoxel{false, false, false}
    {
    }
  };
  NODISCARD btAABB FindMeshAABB(Mesh *mesh);
  // using MeshInfo = std::vector<std::pair<std::array<btVector3, 3>, std::array<u32, 3>>>;

  NODISCARD std::vector<MeshInfo> FindTriangleAABBs(Mesh *mesh);

  NODISCARD VoxObj::VoxelMesh
  GenerateVoxels(std::vector<MeshInfo> &meshTriangles, const btAABB &meshAABB, Mesh *mesh);
  void FillVoxelMesh(VoxObj::VoxelMesh *voxelMesh);
  void AddNeighbors(VoxObj::VoxelMesh *voxelMesh);
};

} // namespace VoxGen