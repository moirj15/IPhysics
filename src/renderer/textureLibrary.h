#pragma once
#include "../common.h"
#include "texture.h"

#include <unordered_map>

class TextureLibrary
{
  std::unordered_map<u32, GLTexture2D> mTextures;

public:
  inline u32 Add(GLTexture2D texture)
  {
    static u32 CURRENT_HANDLE = 0;
    CURRENT_HANDLE++;
    mTextures.insert(std::make_pair(CURRENT_HANDLE, texture));
    return CURRENT_HANDLE;
  }
  void Remove(const u32 handle)
  {
    auto texture = mTextures[handle];
    texture.Destroy();
    mTextures.erase(handle);
  }

  inline GLTexture2D GetProgram(const u32 handle)
  {
    return mTextures[handle];
  }
};
