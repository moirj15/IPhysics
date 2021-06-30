#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <Common.h>

struct SceneMember {
  std::string name = "";
  std::string voxelMeshPath = "";
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::vec3 forceDirection{0.0f, 0.0f, 0.0f};
  f32 force = 0.0f;
};

std::vector<SceneMember> ReadSceneFile(const std::string &path);