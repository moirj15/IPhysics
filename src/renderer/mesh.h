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

  Mesh() = default;
  explicit Mesh(const Mesh &m) :
      mVertecies(m.mVertecies), mNormals(m.mNormals), mTextureCoords(m.mTextureCoords),
      mIndecies(m.mIndecies)
  {
  }

  void Clear()
  {
    mVertecies.clear();
    mNormals.clear();
    mTextureCoords.clear();
    mIndecies.clear();
  }
};
