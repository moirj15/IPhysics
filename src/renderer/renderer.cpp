#include "renderer.h"

#include "../window.h"
#include "meshLibrary.h"
#include "shader.h"
#include "shaderLibrary.h"
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
      auto *shader = GetShader(command.mType);
      shader->Bind();
      shader->SetUniform3F("color", command.mColor);
    }

    break;
    case CommandType::DrawLine:
      break;
    case CommandType::DrawPoints:
      break;
    case CommandType::AddTexture:
      break;
    case CommandType::DrawTextured:
      break;
    }
  }
}

void Renderer::UpdateScreen()
{
  glfwSwapBuffers(mWindow->mGLFWWindow);
}

Shader *Renderer::GetShader(const CommandType type)
{
  switch (type)
  {
  case CommandType::DrawSolid:
    return mShaderLibrary->GetProgram("flatColorShader");
  case CommandType::DrawLine:
    return mShaderLibrary->GetProgram("lineShader");
  case CommandType::DrawPoints:
    return mShaderLibrary->GetProgram("pointShader");
  case CommandType::DrawTextured:
    return mShaderLibrary->GetProgram("texturedShader");
  default:
    assert(0);
  }
}
