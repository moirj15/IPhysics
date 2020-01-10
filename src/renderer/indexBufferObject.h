//
// Created by moirj15 on 1/1/20.
//

#pragma once

#include "../common.h"
#include "util.h"

#include <GL/glew.h>

class IndexBuffer
{
  u32 mCount;
  u32 mHandle;

public:
  IndexBuffer(const u32 *indecies, const u32 len) : mCount(len)
  {
    mHandle = CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, (void *)indecies, len * sizeof(u32));
  }

  inline void Destroy()
  {
    glDeleteBuffers(1, &mHandle);
  }
  inline void Bind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
  }
  inline void Unbind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
};
