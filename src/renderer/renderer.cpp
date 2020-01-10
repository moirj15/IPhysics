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
      auto *shader = mShaderLibrary.GetProgram("flatColorShader");
      shader->Bind();
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
