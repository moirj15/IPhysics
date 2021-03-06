//
// Created by moirj15 on 1/1/20.
//

#pragma once
#if 0

#include "IndexBufferObject.h"
#include "VertexBufferObject.h"

#include <Common.h>
#include <unordered_map>

class VertexArray
{
  u32 mHandle;
  // SHOULD NOT BE ALLOCATED
  IndexBuffer mIndexBuffer;
  std::unordered_map<s32, VertexBuffer> mVertexBuffers;

public:
  VertexArray(const IndexBuffer &indexBuffer) : mHandle(0)
  {
    glGenVertexArrays(1, &mHandle);
  }

  /**
   * \brief: Destroy the VertexArray and any associated IndexBuffers and VertexBuffers.
   */
  inline void Destroy()
  {
    glDeleteVertexArrays(1, &mHandle);
    mIndexBuffer.Destroy();
    for (auto &vb : mVertexBuffers)
    {
      vb.Destroy();
    }
  }

  /**
   * \brief: Bind the VertexArray.
   */
  inline void Bind() const
  {
    glBindVertexArray(mHandle);
    mIndexBuffer.Bind();
  }

  /**
   * \brief: Unbind all VertexArrays.
   */
  inline void Unbind() const
  {
    glBindVertexArray(0);
    mIndexBuffer.Unbind();
  }

  /**
   * \brief: Add a VertexBuffer to the VertexArray.
   * \param vbo: The VertexBuffer.
   */
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

  /**
   * \brief: Add a IndexBuffer to the VertexArray.
   * \param ibo: The IndexBuffer.
   */
  inline void AddIBO(const IndexBuffer &ibo)
  {
    glBindVertexArray(mHandle);
    mIndexBuffer.Destroy();
    mIndexBuffer = ibo;
  }
};
#endif
