#pragma once

#include "../../Common.h"

#include <string>

namespace VoxObj
{
class VoxelMesh;
}

namespace Utils
{
void Serialize(VoxObj::VoxelMesh *voxelMesh);

VoxObj::VoxelMesh *DeSerialize(const std::string &path);
} // namespace Utils
