#pragma once
#include "texture.h"

#include <unordered_map>

class TextureLibrary
{
  std::unordered_map<std::string, Texture2D> mTextures;

public:
  inline void Add(const std::string &name, Texture2D texture)
  {
    mTextures.insert(std::make_pair(name, texture));
  }
  void Remove(const std::string &name)
  {
    auto texture = mTextures[name];
    texture.Destroy();
    mTextures.erase(name);
  }

  inline Texture2D GetProgram(const std::string &name)
  {
    return mTextures[name];
  }
};
