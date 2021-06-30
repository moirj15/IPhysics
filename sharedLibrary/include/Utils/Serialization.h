#pragma once

#include <Common.h>
#include <VoxelObjects/Objects.h>

#include <string>
#include <json.hpp>

namespace glm
{

void from_json(const nlohmann::json &j, vec3 &v);
}

namespace shared
{
void Serialize(const objs::Mesh &mesh, const objs::VoxelMesh &voxelMesh, const std::string &outputPath, const std::string &meshPath, float voxelSize, bool solid);

std::pair<objs::Mesh, objs::VoxelMesh> DeSerialize(const std::string &path);

} // namespace Utils
