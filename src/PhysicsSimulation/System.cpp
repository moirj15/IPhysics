#include "System.h"

#include "../../imgui/imgui.h"
#include "../PhysicsEngine/physics.h"
#include "../renderer/DebugDrawer.h"
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
    mDB(new Renderer::DebugDrawer(mRenderer->GetBackend())),
    mPhysicsEngine(new Physics::PhysicsEngine(mDB))
{
  mUI->Init(mWindow.get());
  mRenderer->SetProjection(mProjection);
}

System::~System() = default;

void System::Run()
{
  auto *vm = Utils::DeSerialize("../test-out/block-1.vmf");
  mHandle = VoxelMeshManager::Get().SubmitMesh(vm);
  auto *vm2 = Utils::DeSerialize("../test-out/block-1.vmf");
  auto handle = VoxelMeshManager::Get().SubmitMesh(vm2);
  auto *os = new Physics::ObjectSettings();
  auto *os2 = new Physics::ObjectSettings();
  os->mPosition = {-2.0, 0.0, 0.0};
  os2->mPosition = {2.0, 0.0, 0.0};
  VoxelMeshManager::Get().SubmitSettings(mHandle, os);
  VoxelMeshManager::Get().SubmitSettings(handle, os2);
  mPhysicsEngine->SubmitObject(mHandle);
  mPhysicsEngine->SubmitObject(handle);

  mRenderer->RegisterMeshHandle(mHandle);
  mRenderer->RegisterMeshHandle(handle);

  while (!mWindow->ShouldClose())
  {

    auto &io = ImGui::GetIO();
    CollectInput();
    mPhysicsEngine->Update(io.DeltaTime);
    Render();
  }
}

void System::CollectInput()
{
  glfwPollEvents();
  auto &io = ImGui::GetIO();
  if (!io.WantCaptureMouse && io.MouseReleased[0] && io.KeysDown[GLFW_KEY_LEFT_CONTROL])
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
  f32 boost = 1.0f;
  if (io.KeysDown[GLFW_KEY_LEFT_SHIFT] && !io.WantCaptureKeyboard)
  {
    boost = 5.0f;
  }
  if (io.KeysDown[GLFW_KEY_W] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(0.0f, 0.0f, 1.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_S] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(0.0f, 0.0f, -1.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_A] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(-1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_D] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_E] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(0.0f, 1.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_Q] && !io.WantCaptureKeyboard)
  {
    mCamera.Move(glm::vec3(0.0f, -1.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (!io.WantCaptureMouse && io.MouseDown[0] && !io.KeysDown[GLFW_KEY_LEFT_CONTROL])
  {
    f32 screenWidth = f32(mWindow->GetWidth());
    f32 screenHeight = f32(mWindow->GetHeight());
    glm::vec2 mouseDelta(
        (screenWidth / 2.0f) - io.MousePos.x, (screenHeight / 2.0f) - io.MousePos.y);
    // glm::vec2 mouseDelta(io.MouseDelta.x, io.MouseDelta.y);
    mCamera.Rotate(mouseDelta);
  }
}

void System::Render()
{
  mRenderer->Clear();
  mRenderer->Draw();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  mDB->Draw(mCamera.CalculateMatrix(), mProjection);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  mUI->Update();
  glfwSwapBuffers(mWindow->mGLWindow);
}
} // namespace IPhysics
