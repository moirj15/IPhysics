#pragma once

#include "../common.h"
#include "../voxelObjects/VoxelMesh.h"
#include "../voxelObjects/voxel.h"
#include "VoxelizerParameters.h"

#include <array>
#include <reactphysics3d.h>
#include <vector>

struct Mesh;

namespace VoxGen
{

class Voxelizer
{
  Parameters mParameters;

public:
  VoxObj::VoxelMesh Voxelize(Mesh *mesh);

  inline void SetParameters(const Parameters &parameters)
  {
    mParameters = parameters;
  }
  inline void Reset()
  {
    mParameters = Parameters();
  }

private:
  rp3d::AABB FindMeshAABB(Mesh *mesh);
  std::vector<std::array<rp3d::Vector3, 3>> FindTriangleAABBs(Mesh *mesh);

  VoxObj::VoxelMesh GenerateVoxels(
      const std::vector<std::array<rp3d::Vector3, 3>> &meshTriangles, const rp3d::AABB &meshAABB,
      Mesh *mesh);
  void FillVoxelMesh(VoxObj::VoxelMesh *voxelMesh);
};

} // namespace VoxGen