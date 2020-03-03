#include "System.h"

#include "../../imgui/imgui.h"
#include "../PhysicsEngine/physics.h"
#include "../renderer/RendererBackend.h"
#include "../renderer/RendererFrontend.h"
#include "../renderer/window.h"
#include "../utils/Serialization.h"
#include "../voxelObjects/VoxelMesh.h"
#include "IPhysicsUI.h"

#include <GLFW/glfw3.h>

namespace IPhysics
{
System::System() :
    mCamera(
        glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    mWindow(Renderer::InitAPI(1980, 1080, "Voxel Generator", false)), mUI(new IPhysicsUI()),
    mRenderer(new Renderer::RendererFrontend(mWindow.get(), &mCamera)),
    mPhysicsEngine(new Physics::PhysicsEngine())
{
  mUI->Init(mWindow.get());
  mRenderer->SetProjection(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f));
}

System::~System() = default;

void System::Run()
{
  auto *vm = Utils::DeSerialize("../test-out/block-1.vmf");
  while (!mWindow->ShouldClose())
  {
    glfwPollEvents();
    auto io = ImGui::GetIO();
    mPhysicsEngine->Update(io.DeltaTime);
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
