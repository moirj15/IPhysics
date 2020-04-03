#pragma once

#include "../common.h"
#include "../voxelObjects/EdgeMesh.h"
#include "../voxelObjects/VoxelMesh.h"
#include "../voxelObjects/voxel.h"
#include "VoxelizerParameters.h"

#include <BulletCollision/Gimpact/btBoxCollision.h>
#include <array>
#include <unordered_map>
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

  NODISCARD std::vector<MeshInfo> FindTriangleAABBs(Mesh *mesh);

  NODISCARD VoxObj::VoxelMesh
  GenerateVoxels(std::vector<MeshInfo> &meshTriangles, const btAABB &meshAABB, Mesh *mesh);
  void FillVoxelMesh(VoxObj::VoxelMesh *voxelMesh);
  void AddNeighbors(VoxObj::VoxelMesh *voxelMesh);
  void AddBezierCurves(VoxObj::VoxelMesh *voxelMesh);
  NODISCARD std::unordered_map<u32, Edge> CreateEdgeMap(VoxObj::VoxelMesh *voxelMesh);
};

} // namespace VoxGen