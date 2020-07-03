#ifndef SHADER_H
#define SHADER_H

#include <Common.h>
#include "ShaderData.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class Shader
{
  u32 mProgramHandle;
  std::vector<std::string> mFilePaths;
  std::unordered_map<std::string, s32> mUniformCache;

public:
  Shader() = default;
  /**
   * \brief: Construct and Compile a shader with the shader files found at the paths in the given
   * path list.
   * \param filePaths: The list of paths leading to the shaders we want compiled.
   */
  Shader(const std::vector<std::string> &filePaths);

  /**
   * \brief: Bind the shader.
   */
  inline void Bind() const
  {
    glUseProgram(mProgramHandle);
  }

  /**
   * \brief: Get the OpenGL Shader program handle.
   * \return
   */
  NODISCARD inline u32 Handle() const
  {
    return mProgramHandle;
  }

  /**
   * \brief: Compile a shader with the shader files found at the paths in the given path list.
   * \param filePaths: The list of paths leading to the shaders we want compiled.
   */
  void Compile(const std::vector<std::string> &filePaths);

  /**
   * \brief: Set a single component uniform variable.
   * \param name: The name of the variable we want to set.
   * \param value: The value we want to set it to.
   */
  void SetUniform1F(const std::string &name, f32 value);
  /**
   * \brief: Set a two component uniform variable.
   * \param name: The name of the variable we want to set.
   * \param value: The value we want to set it to.
   */
  void SetUniform2F(const std::string &name, const glm::vec2 &value);

  /**
   * \brief: Set a three component uniform variable.
   * \param name: The name of the variable we want to set.
   * \param value: The value we want to set it to.
   */
  void SetUniform3F(const std::string &name, const glm::vec3 &value);
  /**
   * \brief: Set a four component uniform variable.
   * \param name: The name of the variable we want to set.
   * \param value: The value we want to set it to.
   */
  void SetUniform4F(const std::string &name, const glm::vec4 &value);

  /**
   * \brief: Set a 3x3 matrix uniform variable.
   * \param name: The name of the variable we want to set.
   * \param value: The value we want to set it to.
   */
  void SetUniformMat3(const std::string &name, const glm::mat3 &value);
  /**
   * \brief: Set a 4x4 matrix uniform variable.
   * \param name: The name of the variable we want to set.
   * \param value: The value we want to set it to.
   */
  void SetUniformMat4(const std::string &name, const glm::mat4 &value);

  /**
   * Upload a single ShaderData object.
   * \param shaderData: The ShaderData we're uploading to the shader.
   */
  void SetShaderData(const ShaderData &shaderData);

  /**
   * Upload a list of ShaderData objects.
   * \param shaderData: The list of ShaderData objects.
   */
  void SetShaderData(const std::vector<ShaderData> &shaderData);

  NODISCARD s32 getAttributeLocation(const char *attribute) const;

private:
  /**
   * \brief: Checks if a variable with the given name is present in the uniform cache. If it is then
   * the location is returned, otherwise the location is found, stored in cache, and then returned.
   * \param name: The name of the variable we want.
   * \return: The location of the variable.
   */
  NODISCARD s32 LocationFromCache(const std::string &name);
  /**
   * \brief: Compile an individual shader file.
   */
  void CompileShader();
  /**
   * \brief: Determines what kind of shaders are present in the filePaths list.
   * \return: The list of shader types.
   */
  NODISCARD std::vector<GLenum> DetermineShaderTypes();
};

#endif
