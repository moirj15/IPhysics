#pragma once

#include <Common.h>
#include <Utils/QuickCastBuffer.h>

#include <glm/mat4x4.hpp>
#include <vector>

struct Mesh
{
  QuickCastBuffer<f32, glm::vec3> mVertices;
  QuickCastBuffer<f32, glm::vec3> mOffsets;
  QuickCastBuffer<f32, glm::vec3> mNormals;
  QuickCastBuffer<f32, glm::vec2> mTextureCoords;
  std::vector<u32> mIndices;

  Mesh() = default;
  explicit Mesh(const Mesh &m) :
      mVertices(m.mVertices), mOffsets(m.mOffsets), mNormals(m.mNormals),
      mTextureCoords(m.mTextureCoords), mIndices(m.mIndices)
  {
  }

  void Clear()
  {
    mVertices.Clear();
    mOffsets.Clear();
    mNormals.Clear();
    mTextureCoords.Clear();
    mIndices.clear();
  }
};
