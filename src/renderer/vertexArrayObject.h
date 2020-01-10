//
// Created by moirj15 on 1/1/20.
//

#pragma once

#include "../common.h"
#include "indexBufferObject.h"
#include "vertexBufferObject.h"

class VertexArray
{
  u32 mHandle;
  IndexBuffer mIndexBuffer;
  std::vector<VertexBuffer> mVertexBuffers;

public:
  inline void Create()
  {
    glGenVertexArrays(1, &mHandle);
  }

  inline void Destroy()
  {
    glDeleteVertexArrays(1, &mHandle);
    mIndexBuffer.Destroy();
    for (auto &vb : mVertexBuffers)
    {
      vb.Destroy();
    }
  }
  inline void Bind() const
  {
    glBindVertexArray(mHandle);
    mIndexBuffer.Bind();
  }
  inline void Unbind() const
  {
    glBindVertexArray(0);
    mIndexBuffer.Unbind();
  }

  inline void AddVBO(const VertexBuffer &vbo)
  {
    glBindVertexArray(mHandle);
    vbo.Bind();
    for (const auto &layout : vbo.Layout())
    {
      glEnableVertexAttribArray(layout.mLocation);
      glVertexAttribPointer(
          layout.mLocation, layout.mElementSize, layout.mType, GL_FALSE, layout.mElementStride,
          (void *)0);
    }
    mVertexBuffers.emplace_back(vbo);
  }

  inline void AddIBO(const IndexBuffer &ibo)
  {
    glBindVertexArray(mHandle);
    mIndexBuffer.Destroy();
    mIndexBuffer = ibo;
  }
};
