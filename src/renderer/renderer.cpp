#include "renderer.h"

#include "../window.h"
#include "camera.h"
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
  glEnable(GL_PROGRAM_POINT_SIZE);
  std::vector<std::string> shaderPaths(
      {"../shaders/flatColorShader.vert", "../shaders/flatColorShader.frag"});
  mShaderLibrary->Add(shaderPaths);
  mShaderLibrary->Add(
      std::vector<std::string>({"../shaders/pointShader.vert", "../shaders/pointShader.frag"}));
  mShaderLibrary->Add(
      std::vector<std::string>({"../shaders/textureShader.vert", "../shaders/textureShader.frag"}));
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

void Renderer::Draw(Camera *camera, const glm::mat4 &projection)
{
  for (const auto &command : mCommandQueue)
  {
    switch (command.mType)
    {
    case CommandType::DrawSolid:
    {
      auto *shader = GetShader(command.mType, true);
      shader->SetUniform3F("color", command.mColor);
      shader->SetUniformMat4("camera", camera->CalculateMatrix());
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_TRIANGLES, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawLine:
    {
      auto *shader = GetShader(command.mType, true);
      shader->SetUniformMat4("camera", camera->CalculateMatrix());
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_LINES, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawPoints:
    {
      auto *shader = GetShader(command.mType, true);
      shader->SetUniformMat4("camera", camera->CalculateMatrix());
      auto ibo = GetBuffersAndBind(command.mMeshHandle);
      glDrawElements(GL_POINTS, ibo.IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
    break;
    case CommandType::DrawTextured:
    {
      auto *shader = GetShader(command.mType, true);
      shader->SetUniformMat4("camera", camera->CalculateMatrix());
      auto texture = mTextureLibrary->GetProgram(command.mTextureHandle);
      texture.Bind();
    }
    break;
    // TODO: consider removing this command and adding a member function instead
    case CommandType::UpdateMesh:
      mMeshLibrary->Update(command.mMesh, command.mMeshHandle);
      break;
    case CommandType::ClearDepthBuffer:
      glClear(GL_DEPTH_BUFFER_BIT);
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
  case CommandType::DrawLine:
    shader = mShaderLibrary->GetProgram("flatColorShader");
    break;
  case CommandType::DrawPoints:
    shader = mShaderLibrary->GetProgram("pointShader");
    break;
  case CommandType::DrawTextured:
    shader = mShaderLibrary->GetProgram("texturedShader");
    break;
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
