#pragma once
#include "../../common.h"
#include "texture.h"

#include <unordered_map>

class TextureLibrary
{
  std::unordered_map<u32, GLTexture2D> mTextures;

public:
  /**
   * \brief: Adds a GLTexture2D to the TextureLibrary.
   * \param texture: The GLTexture2D.
   * \return: A handle to the GLTexture2D.
   */
  inline u32 Add(GLTexture2D texture)
  {
    static u32 CURRENT_HANDLE = 0;
    CURRENT_HANDLE++;
    mTextures.insert(std::make_pair(CURRENT_HANDLE, texture));
    return CURRENT_HANDLE;
  }
  /**
   * \brief: Removes the GLTexture2D associated with the given handle.
   * \param handle: The handle of the GLTexture2D that will be removed.
   */
  void Remove(const u32 handle)
  {
    auto texture = mTextures[handle];
    texture.Destroy();
    mTextures.erase(handle);
  }

  /**
   * \brief: Get the GLTexture2D associated with the given handle.
   * \param handle: The handle of the GLTexture2D.
   * \return: The GLTexture2D.
   */
  inline GLTexture2D GetTexture(const u32 handle)
  {
    return mTextures[handle];
  }
};
