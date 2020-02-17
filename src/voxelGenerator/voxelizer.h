#pragma once

#include "../common.h"
#include "../voxelObjects/VoxelMesh.h"
#include "../voxelObjects/voxel.h"
#include "VoxelizerParameters.h"

#include <reactphysics3d.h>
#include <vector>

struct Mesh;

namespace VoxGen
{

class Voxelizer
{
  Parameters mParameters;

public:
  std::vector<VoxObj::Voxel> Voxelize(Mesh *mesh);

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
  std::vector<rp3d::AABB> FindTriangleAABBs(Mesh *mesh);

  VoxObj::VoxelMesh GenerateVoxels(
      const std::vector<rp3d::AABB> &triangleAABBs, const rp3d::AABB &meshAABB, Mesh *mesh);
};

} // namespace VoxGen