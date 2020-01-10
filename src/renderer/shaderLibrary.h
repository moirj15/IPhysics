#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Shader;

class ShaderLibrary
{
  std::unordered_map<std::string, std::unique_ptr<Shader>> mShaders;

public:
  std::string Add(const std::vector<std::string> &filePaths);
  void Remove(const std::string &name);

  inline Shader *GetProgram(const std::string &name)
  {
    return mShaders[name].get();
  }
};
