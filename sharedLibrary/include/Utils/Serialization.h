#pragma once

#include "../../Common.h"

#include <string>

namespace VoxObj
{
class VoxelMesh;
}

namespace Utils
{
/**
 * \brief: Serializes the given VoxelMesh and stores it in the given file path.
 * \param voxelMesh: The VoxelMesh that will be serialized.
 * \param path: The path to the file that the VoxelMesh will be serialized to.
 */
void Serialize(VoxObj::VoxelMesh *voxelMesh, const std::string &path);

VoxObj::VoxelMesh *DeSerialize(const std::string &path);
} // namespace Utils
