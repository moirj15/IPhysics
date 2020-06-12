#pragma once

#include "../../common.h"

#include <glad/glad.h>

struct Texture2D
{
  s32 mWidth;
  s32 mHeight;
  u32 *mData;

  /**
   * \brief: Construct a Texture2D.
   * \param width: The width of the texture.
   * \param height: The height of the texture.
   * \param data: The array containing the data.
   */
  Texture2D(const s32 width, const s32 height, u32 *data) :
      mWidth(width), mHeight(height), mData(data)
  {
  }
};

class GLTexture2D
{
  u32 mHandle;

public:
  /**
   * \brief: Create a GLTexture2D using an existing Texture2D object.
   * \param texture2D: The Texture2D object we want to use to create the OpenGL Texture.
   */
  inline void Create(const Texture2D *texture2D)
  {
    Create(texture2D->mWidth, texture2D->mHeight, texture2D->mData);
  }
  // TODO: consider adding more parameters for options like wrapping and data layout.
  /**
   * \brief: Create a GLTexture2D.
   * \param width: The width of the texture.
   * \param height: The height of the texture.
   * \param data: The array containing the data.
   */
  inline void Create(const s32 width, const s32 height, const void *data)
  {
    glGenTextures(1, &mHandle);
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  }
  /**
   * \brief: Destroy the OpenGL Texture.
   */
  inline void Destroy()
  {
    glDeleteTextures(1, &mHandle);
  }

  /**
   * \brief: Bind the OpenGL Texture.
   */
  inline void Bind()
  {
    glBindTexture(GL_TEXTURE_2D, mHandle);
  }
};
