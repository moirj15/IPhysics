#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Shader;

class ShaderLibrary
{
  std::unordered_map<std::string, std::unique_ptr<Shader>> mShaders;

public:
  /**
   * \brief: Compile the shaders that are found in the given list of paths.
   * @param filePaths: The list of paths where the shaders can be found.
   * @return: A string that can be used to access the shader.
   */
  std::string Add(const std::vector<std::string> &filePaths);

  /**
   * \brief: Remove a shader from the library with the given name.
   * @param name: The name of the shader that will be removed.
   */
  void Remove(const std::string &name);

  /**
   * \brief: Get the Shader with the given name.
   * @param name: The name of the shader we want to get.
   * @return: A pointer to the shader we want.
   */
  inline Shader *GetProgram(const std::string &name)
  {
    return mShaders[name].get();
  }
};
