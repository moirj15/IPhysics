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
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_LINES, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawPoints:
    {
      auto *shader = GetShader(command.mType, true);
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_POINTS, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawTextured:
    {
      auto *shader = GetShader(command.mType, true);
      auto texture = mTextureLibrary->GetProgram(command.mTextureHandle);
      texture.Bind();
    }
    break;
    case CommandType::UpdateMesh:
      mMeshLibrary->Update(command.mMesh, command.mMeshHandle);
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
u32 Renderer::SubmitMesh(Mesh *mesh)
{
  return mMeshLibrary->Insert(mesh);
}
u32 Renderer::SubmitTexture(Texture2D *texture)
{
  GLTexture2D glTexture2D;
  glTexture2D.Create(texture);
  return mTextureLibrary->Add(glTexture2D);
}
