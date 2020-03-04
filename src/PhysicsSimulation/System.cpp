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
    mProjection(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f)),
    mWindow(Renderer::InitAPI(1980, 1080, "Voxel Generator", false)), mUI(new IPhysicsUI()),
    mRenderer(new Renderer::RendererFrontend(mWindow.get(), &mCamera)),
    mPhysicsEngine(new Physics::PhysicsEngine())
{
  mUI->Init(mWindow.get());
  mRenderer->SetProjection(mProjection);
}

System::~System() = default;

void System::Run()
{
  auto *vm = Utils::DeSerialize("../test-out/block-1.vmf");
  mHandle = VoxelMeshManager::Get().SubmitMesh(vm);
  auto *os = new Physics::ObjectSettings();
  VoxelMeshManager::Get().SubmitSettings(mHandle, os);
  mPhysicsEngine->SubmitObject(mHandle);

  mRenderer->RegisterMeshHandle(mHandle);

  while (!mWindow->ShouldClose())
  {
    glfwPollEvents();

    auto &io = ImGui::GetIO();
    CollectInput();
    mPhysicsEngine->Update(io.DeltaTime);
    Render();
  }
}

void System::CollectInput()
{
  auto &io = ImGui::GetIO();
  if (!io.WantCaptureMouse && io.MouseDown[0])
  {
    f32 screenWidth = f32(mWindow->GetWidth());
    f32 screenHeight = f32(mWindow->GetHeight());
    glm::vec3 rayStartNDC(
        ((io.MousePos.x / screenWidth) - 0.5f) * 2.0f, (io.MousePos.y / screenHeight - 0.5f) * 2.0f,
        -1.0);
    glm::vec3 rayEndNDC(
        ((io.MousePos.x / screenWidth) - 0.5f) * 2.0f, (io.MousePos.y / screenHeight - 0.5f) * 2.0f,
        0.0);

    auto invProjCamera = glm::inverse(mProjection * mCamera.CalculateMatrix());

    auto rayStartWorld = invProjCamera * glm::vec4(rayStartNDC, 1.0f);
    rayStartWorld /= rayStartWorld.w;

    auto rayEndWorld = invProjCamera * glm::vec4(rayEndNDC, 1.0f);
    rayEndWorld /= rayEndWorld.w;

    auto rayDirWorld = glm::normalize(rayEndWorld - rayStartWorld);

    mPhysicsEngine->CastRayWithForce(rayStartWorld, rayDirWorld, 1.0f);
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
