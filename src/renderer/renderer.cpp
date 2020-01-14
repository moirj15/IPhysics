#include "renderer.h"

#include "../window.h"
#include "meshLibrary.h"
#include "shader.h"
#include "shaderLibrary.h"
#include "texture.h"
#include "textureLibrary.h"

#include <GLFW/glfw3.h>

Renderer::Renderer(Window *window) :
    mWindow(window), mShaderLibrary(new ShaderLibrary()), mTextureLibrary(new TextureLibrary()),
    mMeshLibrary(new MeshLibrary())
{
}

void Renderer::Draw()
{
  for (const auto &command : mCommandQueue)
  {
    switch (command.mType)
    {
    case CommandType::DrawSolid:
    {
      auto *shader = GetShader(command.mType, true);
      shader->SetUniform3F("color", command.mColor);
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_TRIANGLES, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawLine:
    {
      auto *shader = GetShader(command.mType, true);
      shader->Bind();
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_LINES, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawPoints:
    {
      auto *shader = GetShader(command.mType, true);
      shader->Bind();
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_POINTS, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::AddTexture:
    {
      GLTexture2D glTexture2D;
      glTexture2D.Create(command.mTexture2D);
      mTextureLibrary->Add("Texture, cause I lazy", glTexture2D);
    }
    break;
    case CommandType::DrawTextured:
      break;
    case CommandType::AddMesh:
      mMeshLibrary->Insert(command.mMesh);
      break;
    case CommandType::UpdateMesh:
      break;
    }
  }
}

void Renderer::UpdateScreen()
{
  glfwSwapBuffers(mWindow->mGLFWWindow);
}

Shader *Renderer::GetShader(const CommandType type, bool bind)
{
  Shader *shader = nullptr;
  switch (type)
  {
  case CommandType::DrawSolid:
    shader = mShaderLibrary->GetProgram("flatColorShader");
  case CommandType::DrawLine:
    shader = mShaderLibrary->GetProgram("lineShader");
  case CommandType::DrawPoints:
    shader = mShaderLibrary->GetProgram("pointShader");
  case CommandType::DrawTextured:
    shader = mShaderLibrary->GetProgram("texturedShader");
  default:
    assert(0);
  }
  if (bind)
  {
    shader->Bind();
  }
  return shader;
}
IndexBuffer Renderer::GetBuffersAndBind(const u32 handle)
{
  auto [vao, ibo] = (*mMeshLibrary)[handle];
  vao.Bind();
  ibo.Bind();
  return ibo;
}
