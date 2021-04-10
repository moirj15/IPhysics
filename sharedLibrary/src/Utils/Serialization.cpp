#include <Utils/Serialization.h>
#include <filesystem>
#include <fstream>
#include <glm/vec3.hpp>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

namespace glm
{

void to_json(json &j, const vec3 &v)
{
  j = json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}

void from_json(const json &j, vec3 &v)
{
  v = vec3(j["x"].get<f32>(), j["y"].get<f32>(), j["z"].get<f32>());
}

void to_json(json &j, const uvec3 &v)
{
  j = json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}

void from_json(const json &j, uvec3 &v)
{
  v = uvec3(j["x"].get<u32>(), j["y"].get<u32>(), j["z"].get<u32>());
}

} // namespace glm

namespace objs
{

void to_json(json &j, const BezierCurve &b)
{
  j = json{{"control_points", b.controlPoints}, {"first_t", b.firstT}, {"second_t", b.secondT},
      {"effected_points", b.effectedPoints}};
}

void from_json(const json &j, BezierCurve &b)
{
  b = {
      .controlPoints = j["control_points"],
      .firstT = j["first_t"],
      .secondT = j["second_t"],
      .effectedPoints = j["effected_points"],
  };
}

void to_json(json &j, const Voxel &v)
{
  j = json{{"size", v.size}, {"dimmensions", v.dimmensions},
      {"position_relative_to_center", v.positionRelativeToCenter}, {"neighbors", v.neighbors},
      {"bezier_curves", v.bezierCurves}};
}

void from_json(const json &j, Voxel &v)
{
  v = {
      .size = j["size"].get<glm::vec3>(),
      .dimmensions = j["dimmensions"].get<glm::vec3>(),
      .positionRelativeToCenter = j["position_relative_to_center"].get<glm::vec3>(),
      .neighbors = j["neighbors"],
      .bezierCurves = j["bezier_curves"],
  };
}
// clang-format on

} // namespace objs

namespace shared
{

void Serialize(const objs::Mesh &mesh, const objs::VoxelMesh &voxelMesh, const std::string &outputPath,
    const std::string &meshPath, float voxelSize, bool solid)
{
  json out;

  json header;
  header["mesh_path"] = meshPath;
  header["voxel_size"] = voxelSize;
  header["is_solid"] = solid;

  out["header"] = header;

  json serializedMesh;

  serializedMesh["vertices"] = mesh.vertices;
  serializedMesh["normals"] = mesh.normals;
  serializedMesh["indices"] = mesh.indices;

  out["mesh"] = serializedMesh;

  json serializedVoxelMesh;

  serializedVoxelMesh["dimmensions_voxel_space"] = voxelMesh.dimensionsVoxelSpace;
  serializedVoxelMesh["dimmensions_object_space"] = voxelMesh.dimensionsObjectSpace;
  serializedVoxelMesh["initial_voxel_size"] = voxelMesh.initialVoxelSize;
  serializedVoxelMesh["voxels"] = voxelMesh.voxels;

  out["voxel_mesh"] = serializedVoxelMesh;

  // std::cout << std::setw(4) << out << std::endl;
  std::string currPath = (char *)std::filesystem::current_path().c_str();
  std::ofstream outputStream(outputPath);
  outputStream << std::setw(4) << out << std::endl;
}

std::pair<objs::Mesh, objs::VoxelMesh> DeSerialize(const std::string &path)
{
  json in;
  std::ifstream inputStream(path);
  inputStream >> in;
  json serializedMesh = in["mesh"];
  objs::Mesh mesh = {
      .vertices = serializedMesh["vertices"],
      .normals = serializedMesh["normals"],
      .indices = serializedMesh["indices"],
  };

  json serializedVoxelMesh = in["voxel_mesh"];
  objs::VoxelMesh voxelMesh = {
      .dimensionsVoxelSpace = serializedVoxelMesh["dimmensions_voxel_space"],
      .dimensionsObjectSpace = serializedVoxelMesh["dimmensions_object_space"],
      .initialVoxelSize = serializedVoxelMesh["initial_voxel_size"],
      .voxels = serializedVoxelMesh["voxels"],
  };
  return {mesh, voxelMesh};
}

} // namespace shared