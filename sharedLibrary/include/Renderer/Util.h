//
// Created by moirj15 on 1/1/20.
//

#pragma once

#include <Common.h>

#include <glad/glad.h>

/**
 * \brief: Create an OpenGL Buffer object.
 * \param bufferType: The type of buffer.
 * \param data: The list of data that will be stored in the buffer.
 * \param len: The length of the data list.
 * \return: The handle to the OpenGL buffer.
 */
inline u32 CreateBuffer(GLenum bufferType, void *data, u32 len, bool dynamic = false)
{
  u32 handle = 0;
  glGenBuffers(1, &handle);
  glBindBuffer(bufferType, handle);
  if (dynamic)
  {
    glBufferData(bufferType, len, data, GL_DYNAMIC_DRAW);
  }
  else
  {
    glBufferData(bufferType, len, data, GL_STATIC_DRAW);
  }
  return handle;
}
