#pragma once

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>

struct ShaderData
{
  std::string name;

  enum class DataType
  {
    Vec2,
    Vec3,
    Vec4,
    Mat3,
    Mat4
  } type;
  union
  {
    glm::vec2 v2;
    glm::vec3 v3;
    glm::vec4 v4;
    glm::mat3 m3;
    glm::mat4 m4;
  };

  ShaderData() = default;
  ShaderData(const std::string &n, const glm::vec2 &v) : name(n), type(DataType::Vec2), v2(v)
  {
  }
  ShaderData(const std::string &n, const glm::vec3 &v) : name(n), type(DataType::Vec3), v3(v)
  {
  }
  ShaderData(const std::string &n, const glm::vec4 &v) : name(n), type(DataType::Vec4), v4(v)
  {
  }
  ShaderData(const std::string &n, const glm::mat3 &m) : name(n), type(DataType::Mat3), m3(m)
  {
  }
  ShaderData(const std::string &n, const glm::mat4 &m) : name(n), type(DataType::Mat4), m4(m)
  {
  }
};
