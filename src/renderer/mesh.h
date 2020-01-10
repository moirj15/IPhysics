#pragma once

#include "../common.h"

#include <glm/mat4x4.hpp>
#include <vector>

struct Mesh
{
  std::vector<f32> mVertecies;
  std::vector<f32> mNormals;
  std::vector<f32> mTextureCoords;
  std::vector<u32> mIndecies;
  std::vector<glm::mat4> mTransforms;
};
