#include "Scene.h"

#include <json.hpp>
#include <fstream>
#include <Utils/Serialization.h>

//namespace glm
//{
//
//void from_json(const nlohmann::json &j, vec3 &v)
//{
//  v = vec3(j["x"].get<f32>(), j["y"].get<f32>(), j["z"].get<f32>());
//}
//
//} // namespace glm

std::vector<SceneMember> ReadSceneFile(const std::string &path)
{
  nlohmann::json sceneFile;
  std::ifstream inputStream(path);
  inputStream >> sceneFile;
  std::vector<SceneMember> sceneMembers;
  for (const auto &entry : sceneFile) {
    sceneMembers.push_back({
        .name = entry["name"].get<std::string>(),
        .voxelMeshPath = entry["object_file_path"].get<std::string>(),
        .position = entry["position"].get<glm::vec3>(),
        .forceDirection = entry["force_direction"].get<glm::vec3>(),
        .force = entry["force"],
    });
  }
  return sceneMembers;
}
