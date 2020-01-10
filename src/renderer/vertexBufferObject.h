//
// Created by moirj15 on 1/1/20.
//

#pragma once
#include "../common.h"
#include "util.h"

#include <GL/glew.h>
#include <string>
#include <vector>

struct BufferLayout
{
  std::string mName;
  u32 mElementSize;
  u32 mElementStride;
  u32 mLocation;
  GLenum mType;

  BufferLayout(
      const std::string &name, const u32 elementSize, const u32 elementStride, const u32 location,
      const GLenum type) :
      mName(name),
      mElementSize(elementSize), mElementStride(elementStride), mLocation(location), mType(type)
  {
  }
};

class VertexBuffer
{
  u32 mHandle;
  std::vector<BufferLayout> mLayout;

public:
  VertexBuffer(const f32 *vertexes, const u32 len, const std::vector<BufferLayout> &layout) :
      mLayout(layout)
  {
    mHandle = CreateBuffer(GL_ARRAY_BUFFER, (void *)vertexes, len * sizeof(f32));
  }

  inline void Destroy()
  {
    glDeleteBuffers(1, &mHandle);
  }
  inline void Bind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, mHandle);
  }
  inline void Unbind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  inline const std::vector<BufferLayout> &Layout() const
  {
    return mLayout;
  }
};
