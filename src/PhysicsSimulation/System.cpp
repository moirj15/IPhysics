#include "System.h"

#include "../renderer/RendererBackend.h"
#include "../renderer/RendererFrontend.h"
#include "../renderer/window.h"
#include "IPhysicsUI.h"

#include <GLFW/glfw3.h>

namespace IPhysics
{
System::System() :
    mCamera(
        glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    mWindow(Renderer::InitAPI(1980, 1080, "Voxel Generator", false)), mUI(new IPhysicsUI()),
    mRenderer(new Renderer::RendererFrontend(mWindow.get(), &mCamera))
{
  mUI->Init(mWindow.get());
  mRenderer->SetProjection(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f));
}

System::~System() = default;

void System::Run()
{
  while (!mWindow->ShouldClose())
  {
    glfwPollEvents();
    Render();
  }
}

void System::Render()
{
  mRenderer->Clear();
  mRenderer->Draw();
  mUI->Update();
  glfwSwapBuffers(mWindow->mGLWindow);
}
} // namespace IPhysics
