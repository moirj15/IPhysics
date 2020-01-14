#pragma once

#include "../common.h"

#include <GL/glew.h>

struct Texture2D
{
  s32 mWidth;
  s32 mHeight;
  u32 *mData;
  Texture2D(const s32 width, const s32 height, u32 *data) :
      mWidth(width), mHeight(height), mData(data)
  {
  }
};

class GLTexture2D
{
  u32 mHandle;

public:
  inline void Create(const Texture2D *texture2D)
  {
    Create(texture2D->mWidth, texture2D->mHeight, texture2D->mData);
  }
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
