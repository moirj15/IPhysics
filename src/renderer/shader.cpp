#include "shader.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

static char *readFile(const char *name)
{
  FILE *fp = nullptr;
  fp = fopen(name, "r");
  char *src = nullptr;
  uint64_t len = 0;

  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  rewind(fp);

  if (len == 0)
  {
    fprintf(stderr, "failed to get file size");
    // exit(EXIT_FAILURE);
  }

  src = (char *)calloc(len + 1, sizeof(char));
  len = fread(src, sizeof(char), len, fp);

  fclose(fp);

  return src;
}

static void printShaderInfo(u32 shader)
{
  s32 length = 0;
  GLsizei nchars = 0;
  char *log = nullptr;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

  if (length > 0)
  {
    log = new char[length];

    if (log != nullptr)
    {
      glGetShaderInfoLog(shader, length, &nchars, log);

      if (log[0] != '\0')
      {
        printf("shader log: '%s'\n", log);
      }

      delete[](log);
    }
  }
}

static void printProgramProgramInfo(const u32 shader)
{
  GLint length = 0;
  GLsizei nchars = 0;
  char *log = nullptr;

  glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);

  if (length > 0)
  {
    log = new char[length];

    if (log)
    {
      glGetProgramInfoLog(shader, length, &nchars, log);

      if (log[0] != '\0')
      {
        printf("program log: '%s'\n", log);
      }

      delete[](log);
    }
  }
}

Shader::Shader(const std::vector<std::string> &filePaths) : mProgramHandle(0), mFilePaths(filePaths)
{
  CompileShader();
}

void Shader::Compile(const std::vector<std::string> &filePaths)
{
  mProgramHandle = 0;
  mFilePaths = filePaths;
  CompileShader();
}

void Shader::SetUniform1F(const std::string &name, const f32 value)
{
  const s32 location = LocationFromCache(name);
  glUniform1f(location, value);
}

void Shader::SetUniform2F(const std::string &name, const glm::vec2 &value)
{
  const s32 location = LocationFromCache(name);
  glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniform3F(const std::string &name, const glm::vec3 &value)
{
  const s32 location = LocationFromCache(name);
  glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniform4F(const std::string &name, const glm::vec4 &value)
{
  const s32 location = LocationFromCache(name);
  glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniformMat3(const std::string &name, const glm::mat3 &value)
{
  const s32 location = LocationFromCache(name);
  glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
}

void Shader::SetUniformMat4(const std::string &name, const glm::mat4 &value)
{
  const s32 location = LocationFromCache(name);
  glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
}

void Shader::SetShaderData(const ShaderData &shaderData)
{
  switch (shaderData.type)
  {
  case ShaderData::Vec2:
    SetUniform2F(shaderData.name, shaderData.v2);
    break;
  case ShaderData::Vec3:
    SetUniform3F(shaderData.name, shaderData.v3);
    break;
  case ShaderData::Vec4:
    SetUniform4F(shaderData.name, shaderData.v4);
    break;
  case ShaderData::Mat3:
    SetUniformMat3(shaderData.name, shaderData.m3);
    break;
  case ShaderData::Mat4:
    SetUniformMat4(shaderData.name, shaderData.m4);
    break;
  }
}

void Shader::SetShaderData(const std::vector<ShaderData> &shaderData)
{
  for (const auto &sd : shaderData)
  {
    SetShaderData(sd);
  }
}

// Private
s32 Shader::LocationFromCache(const std::string &name)
{
  if (mUniformCache.find(name) != mUniformCache.end())
  {
    return mUniformCache[name];
  }
  else
  {
    mUniformCache[name] = glGetUniformLocation(mProgramHandle, name.c_str());
    return mUniformCache[name];
  }
}

void Shader::CompileShader()
{
  mProgramHandle = glCreateProgram();
  s32 flag;
  std::vector<GLenum> shaderTypes = DetermineShaderTypes();

  for (Size i = 0; i < mFilePaths.size(); i++)
  {
    const auto filename = mFilePaths[i].c_str();
    const auto shaderType = shaderTypes[i];
    char *shaderSrc = nullptr;

    const u32 shaderRef = glCreateShader(shaderType);

    shaderSrc = readFile(filename);

    glShaderSource(shaderRef, 1, static_cast<GLchar **>(&shaderSrc), nullptr);

    delete[](shaderSrc);

    glCompileShader(shaderRef);
    glGetShaderiv(shaderRef, GL_COMPILE_STATUS, &flag);
    printShaderInfo(shaderRef);

    if (!flag)
    {
      fprintf(stderr, "failed to compile frag shader\n");
      // exit(EXIT_FAILURE);
    }

    glAttachShader(mProgramHandle, shaderRef);
  }
  glLinkProgram(mProgramHandle);
  glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &flag);

  printProgramProgramInfo(mProgramHandle);

  if (!flag)
  {
    fprintf(stderr, "failed to link shaders");
    // exit(EXIT_FAILURE);
  }
}

std::vector<GLenum> Shader::DetermineShaderTypes()
{
  std::vector<GLenum> ret;
  for (const auto &src : mFilePaths)
  {
    std::string type = src.substr(src.size() - 4);
    if (type == "vert")
    {
      ret.push_back(GL_VERTEX_SHADER);
    }
    else if (type == "frag")
    {
      ret.push_back(GL_FRAGMENT_SHADER);
    }
  }
  return ret;
}
