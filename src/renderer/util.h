//
// Created by moirj15 on 1/1/20.
//

#pragma once

#include "../common.h"
#include <GL/glew.h>

inline u32 CreateBuffer(GLenum bufferType, void *data, u32 len)
{
  u32 handle = 0;
  glGenBuffers(1, &handle);
  glBindBuffer(bufferType, handle);
  glBufferData(bufferType, len, data, GL_STATIC_DRAW);
  return handle;
}


