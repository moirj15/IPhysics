#ifndef SHADER_H
#define SHADER_H

#include "../common.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <unordered_map>
#include <vector>

struct ShaderData
{
  std::string name;

  enum
  {
    Vec2,
    Vec3,
    Vec4,
    Mat3,
    Mat4
  } type;
  union {
    glm::vec2 v2;
    glm::vec3 v3;
    glm::vec4 v4;
    glm::mat3 m3;
    glm::mat4 m4;
  };

  ShaderData() = default;
  ShaderData(const std::string &n, const glm::vec2 &v) : name(n), type(Vec2), v2(v)
  {
  }
  ShaderData(const std::string &n, const glm::vec3 &v) : name(n), type(Vec3), v3(v)
  {
  }
  ShaderData(const std::string &n, const glm::vec4 &v) : name(n), type(Vec4), v4(v)
  {
  }
  ShaderData(const std::string &n, const glm::mat3 &m) : name(n), type(Mat3), m3(m)
  {
  }
  ShaderData(const std::string &n, const glm::mat4 &m) : name(n), type(Mat4), m4(m)
  {
  }
};

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
   * \brief: Unbind all shaders.
   */
  inline void Unbind() const
  {
    glUseProgram(0);
  }

  /**
   * \brief: Get the OpenGL Shader program handle.
   * \return
   */
  inline u32 Handle() const
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
  void SetUniform1F(const std::string &name, const f32 value);
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

private:
  /**
   * \brief: Checks if a variable with the given name is present in the uniform cache. If it is then
   * the location is returned, otherwise the location is found, stored in cache, and then returned.
   * \param name: The name of the variable we want.
   * \return: The location of the variable.
   */
  s32 LocationFromCache(const std::string &name);
  /**
   * \brief: Compile an individual shader file.
   */
  void CompileShader();
  /**
   * \brief: Determines what kind of shaders are present in the filePaths list.
   * \return: The list of shader types.
   */
  std::vector<GLenum> DetermineShaderTypes();
};

#endif
