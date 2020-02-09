#pragma once

#include "../common.h"
#include "../voxelObjects/voxel.h"

#include <reactphysics3d.h>
#include <vector>

struct Mesh;

namespace VoxGen
{

class Voxelizer
{

public:
  std::vector<vx::Voxel> Voxelize(Mesh *mesh, f32 size, bool isHollow);

private:
  rp3d::AABB FindMeshAABB(Mesh *mesh);
  std::vector<rp3d::AABB> FindTriangleAABBs(Mesh *mesh);
};

} // namespace vg