#pragma once

#include "../common.h"

#include <GL/glew.h>

class Texture2D
{
  u32 mHandle;

public:
  // TODO: consider adding more parameters for options like wrapping and data layout.
  inline void Create(const s32 width, const s32 height, const void *data)
  {
    glGenTextures(1, &mHandle);
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  }
  inline void Destroy()
  {
    glDeleteTextures(1, &mHandle);
  }

  inline void Bind()
  {
    glBindTexture(GL_TEXTURE_2D, mHandle);
  }
};
