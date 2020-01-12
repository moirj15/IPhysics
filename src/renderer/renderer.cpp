#include "renderer.h"

#include "../window.h"
#include "shader.h"

#include <GLFW/glfw3.h>

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
    return mShaderLibrary.GetProgram("flatColorShader");
  case CommandType::DrawLine:
    return mShaderLibrary.GetProgram("lineShader");
  case CommandType::DrawPoints:
    return mShaderLibrary.GetProgram("pointShader");
  case CommandType::DrawTextured:
    return mShaderLibrary.GetProgram("texturedShader");
  default:
    assert(0);
  }
}
