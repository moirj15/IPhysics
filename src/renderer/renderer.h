
#pragma once

#include "../common.h"

#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>

struct Mesh;
struct Window;
class Shader;
class ShaderLibrary;
class TextureLibrary;
class MeshLibrary;

enum class CommandType : u32
{
  DrawSolid,
  DrawLine,
  DrawPoints,
  AddTexture,
  DrawTextured,
  UpdateMesh,
};

struct Command
{
  CommandType mType;
  u32 mMeshHandle = 0;
  u32 mTextureHandle = 0;
  Mesh *mMesh = nullptr;
  glm::vec3 mColor = {0.0f, 0.0f, 0.0f};
  Command(
      const CommandType type, const u32 meshHandle = 0, const u32 textureHandle = 0,
      Mesh *mesh = nullptr, const glm::vec3 &color = {0.0f, 0.0f, 0.0f}) :
      mType(type),
      mMeshHandle(meshHandle), mTextureHandle(textureHandle), mMesh(mesh), mColor(color)
  {
  }
};

// struct Command
//{
//  CommandType mType;
//  Mesh *mMesh;
//  Texture2D *mTexture;
//  std::string mTextureName;
//  glm::vec3 mColor;

//  Command(
//      const CommandType type, Mesh *mesh = nullptr, Texture2D *texture = nullptr,
//      const std::string textureName = "", const glm::vec3 &color = glm::vec3(0.0f)) :
//      mType(type),
//      mMesh(mesh), mTexture(texture), mTextureName(textureName), mColor(color)
//  {
//  }
//};

class Renderer
{
  Window *mWindow;
  std::vector<Command> mCommandQueue;
  std::unique_ptr<ShaderLibrary> mShaderLibrary;
  std::unique_ptr<TextureLibrary> mTextureLibrary;
  std::unique_ptr<MeshLibrary> mMeshLibrary;

public:
  explicit Renderer(Window *window);
  inline void SubmitCommand(Command &command)
  {
    mCommandQueue.emplace_back(command);
  }
  inline void ClearCommandQueue()
  {
    mCommandQueue.clear();
  }
  void Draw();
  void UpdateScreen();

private:
  Shader *GetShader(const CommandType type);
};
