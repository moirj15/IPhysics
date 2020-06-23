//
// Created by moirj15 on 1/1/20.
//

#pragma once

#include <Common.h>
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"

class VertexArray
{
  u32 mHandle;
  // TODO: add an accessor for the index buffer
  IndexBuffer mIndexBuffer;
  std::vector<VertexBuffer> mVertexBuffers;

public:
  VertexArray() = default;

  inline const std::vector<VertexBuffer> &GetVertexBuffers() const
  {
    return mVertexBuffers;
  }

  /**
   * \brief: Create a VertexArray.
   */
  inline void Create()
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