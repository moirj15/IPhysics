//
// Created by moirj15 on 12/31/19.
//

#pragma once

#include "../common.h"
#include "shaderLibrary.h"
#include "textureLibrary.h"

#include <glm/vec3.hpp>
#include <string>
#include <vector>

struct Mesh;
struct Texture;
struct Window;

enum class CommandType : u32
{
  DrawSolid,
  DrawLine,
  DrawPoints,
  AddTexture,
  DrawTextured,
};

struct Command
{
  CommandType mType;
  Mesh *mMesh;
  Texture *mTexture;
  std::string mTextureName;
  glm::vec3 color;

  Command(const CommandType type, Mesh *mesh, Texture *texture, const std::string textureName) :
      mType(type), mMesh(mesh), mTexture(texture), mTextureName(textureName)
  {
  }

  Command(Command &command) :
      mType(command.mType), mMesh(command.mMesh), mTexture(command.mTexture),
      mTextureName(std::move(command.mTextureName))
  {
  }
};

class Renderer
{
  Window *mWindow;
  std::vector<Command> mCommandQueue;
  ShaderLibrary mShaderLibrary;
  TextureLibrary mTextureLibrary;

public:
  explicit Renderer(Window *window) : mWindow(window)
  {
  }
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
};
