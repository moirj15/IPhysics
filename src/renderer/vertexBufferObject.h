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

  /**
   * \brief: Construct a BufferLayout.
   * \param name: The name of the layout variable.
   * \param elementSize: The size of the elements in the buffer.
   * \param elementStride: The stride between elements in the buffer.
   * \param location: The location of the buffer in the shader.
   * \param type: The type of value contained in the buffer.
   */
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
  /**
   * \brief: Construct a VertexBuffer.
   * \param vertexes: The list of vertexes.
   * \param len: The length of the list.
   * \param layout: The VertexBuffer's layout.
   */
  VertexBuffer(const f32 *vertexes, const u32 len, const BufferLayout &layout)
  {
    mLayout.emplace_back(layout);
    mHandle = CreateBuffer(GL_ARRAY_BUFFER, (void *)vertexes, len * sizeof(f32));
  }
  /**
   * \brief: Construct a VertexBuffer.
   * \param vertexes: The list of vertexes.
   * \param len: The length of the list.
   * \param layout: The list of the VertexBuffer's layout.
   */
  VertexBuffer(const f32 *vertexes, const u32 len, const std::vector<BufferLayout> &layout) :
      mLayout(layout)
  {
    mHandle = CreateBuffer(GL_ARRAY_BUFFER, (void *)vertexes, len * sizeof(f32));
  }

  /**
   * \brief: Destroy the VertexBuffer.
   */
  inline void Destroy()
  {
    glDeleteBuffers(1, &mHandle);
  }
  /**
   * \brief: Bind the VertexBuffer.
   */
  inline void Bind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, mHandle);
  }
  /**
   * \brief: Unbind all VertexBuffers.
   */
  inline void Unbind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  /**
   * \brief: Get the list of BufferLayouts.
   * \return: The list of BufferLayouts.
   */
  inline const std::vector<BufferLayout> &Layout() const
  {
    return mLayout;
  }
};
