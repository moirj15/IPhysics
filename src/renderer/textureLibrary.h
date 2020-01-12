#pragma once
#include "texture.h"

#include <unordered_map>

class TextureLibrary
{
  std::unordered_map<std::string, GLTexture2D> mTextures;

public:
  inline void Add(const std::string &name, GLTexture2D texture)
  {
    mTextures.insert(std::make_pair(name, texture));
  }
  void Remove(const std::string &name)
  {
    auto texture = mTextures[name];
    texture.Destroy();
    mTextures.erase(name);
  }

  inline GLTexture2D GetProgram(const std::string &name)
  {
    return mTextures[name];
  }
};
