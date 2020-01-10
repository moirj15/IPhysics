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
  Shader(const std::vector<std::string> &filePaths);

  inline void Bind() const
  {
    glUseProgram(mProgramHandle);
  }
  inline void Unbind() const
  {
    glUseProgram(0);
  }

  inline u32 Handle() const
  {
    return mProgramHandle;
  }

  void Compile(const std::vector<std::string> &filePaths);

  void SetUniform1F(const std::string &name, const f32 value);
  void SetUniform2F(const std::string &name, const glm::vec2 &value);
  void SetUniform3F(const std::string &name, const glm::vec3 &value);
  void SetUniform4F(const std::string &name, const glm::vec4 &value);

  void SetUniformMat3(const std::string &name, const glm::mat3 &value);
  void SetUniformMat4(const std::string &name, const glm::mat4 &value);

  void SetShaderData(const ShaderData &shaderData);
  void SetShaderData(const std::vector<ShaderData> &shaderData);

private:
  s32 LocationFromCache(const std::string &name);
  void CompileShader();
  std::vector<GLenum> DetermineShaderTypes();
};

#endif
