#include "shaderLibrary.h"

#include "shader.h"

std::string ShaderLibrary::Add(const std::vector<std::string> &filePaths)
{
  auto *shader = new Shader(filePaths);
  auto name = filePaths.back().substr(
      filePaths.back().find_last_of("/") + 1,
      (filePaths.back().find_last_of(".") - 1) - filePaths.back().find_last_of("/"));
  mShaders.emplace(name, shader);
  return name;
}

void ShaderLibrary::Remove(const std::string &name)
{
  mShaders.erase(name);
}