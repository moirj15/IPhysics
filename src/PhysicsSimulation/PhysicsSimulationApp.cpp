#include "PhysicsSimulationApp.h"

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
PhysicsSimulationApp::PhysicsSimulationApp() :
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

PhysicsSimulationApp::~PhysicsSimulationApp() = default;

void PhysicsSimulationApp::Run()
{

  while (!mWindow->ShouldClose())
  {
    auto &io = ImGui::GetIO();
    LoadObject();
    CollectInput();
    CollectUIInput();
    if (mPhysicsSimulationRunning)
    {
      mPhysicsEngine->Update(io.DeltaTime);
      ApplyDeformations();
    }
    Render();
  }
}

void PhysicsSimulationApp::LoadObject()
{
  auto optionalPath = mUI->LoadObjectClicked();
  if (optionalPath && fs::exists(*optionalPath))
  {
    auto *voxelMesh = Utils::DeSerialize(*optionalPath);
    u32 handle = VoxelMeshManager::Get().SubmitMesh(voxelMesh);
    // TODO: Modify the physics engine so it takes object setting modifications into account
    //     mPhysicsEngine->SubmitObject(handle);
    mRenderer->RegisterMeshHandle(handle);
    mUI->SetCurrentObject(handle);
  }
}

void PhysicsSimulationApp::CollectInput()
{
  glfwPollEvents();
  auto &io = ImGui::GetIO();

  // Check if we want to apply a force with the mouse
  if (!io.WantCaptureMouse && io.MouseReleased[0] && io.KeysDown[GLFW_KEY_LEFT_CONTROL])
  {
    f32 screenWidth = f32(mWindow->GetWidth());
    f32 screenHeight = f32(mWindow->GetHeight());
    // Calculate the mouse position in normalized device coordinates
    const glm::vec2 mouseNDC(
        ((io.MousePos.x / screenWidth) - 0.5f) * 2.0f,
        -((io.MousePos.y / screenHeight) - 0.5f) * 2.0f);
    glm::vec3 rayStartNDC(mouseNDC, 0.0);
    glm::vec3 rayEndNDC(mouseNDC, 1.0);

    // Convert the ray start and end from NDC to world space
    auto invProjCamera = glm::inverse(mProjection * mCamera.CalculateMatrix());

    mPhysicsEngine->CastRayWithForce(rayStartNDC, rayEndNDC, invProjCamera, 1.0f);
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
    mCamera.Rotate(mouseDelta * 10.0f * io.DeltaTime);
  }
}

void PhysicsSimulationApp::CollectUIInput()
{
  auto handle = mUI->CurrentObject();
  if (handle != 0 && mUI->SettingsFieldModified())
  {
    VoxelMeshManager::Get().UpdateOriginalSettings(handle, mUI->GetCurrentObjectsSettings());
  }
  if (mUI->StartSimulationClicked())
  {
    mPhysicsSimulationRunning = true;
    mPhysicsEngine->SetEngineSettings(mUI->GetPhysicsSettings());
    for (auto &[handle, setting] : mUI->GetAllObjectSettings())
    {
      VoxelMeshManager::Get().UpdateOriginalSettings(handle, setting);
      mPhysicsEngine->SubmitObject(handle);
    }
  }
  if (mUI->StopSimulationClicked())
  {
    mPhysicsSimulationRunning = false;
  }
  if (mUI->ResetSimulationClicked())
  {
    mPhysicsSimulationRunning = false;
    mPhysicsEngine->Reset();
    VoxelMeshManager::Get().RestoreSettings();
    for (auto &[handle, _] : mUI->GetAllObjectSettings())
    {
      mPhysicsEngine->SubmitObject(handle);
    }
  }
}

void PhysicsSimulationApp::ApplyDeformations()
{
  for (auto &[handle, vMesh, settings] : VoxelMeshManager::Get().GetAllMeshes())
  {
    for (const auto &[key, voxel] : vMesh->mVoxels)
    {
      for (auto index : voxel.mMeshVertices)
      {
        // glm::vec3 offset = voxel.mPosition - settings->mPosition;
        vMesh->mMesh->mVertices.AccessCastBuffer(index) += voxel.mRelativePositionDelta;
        //         vMesh->mMesh->mVertices[(index * 3)] += voxel.mRelativePositionDelta.x;
        //         vMesh->mMesh->mVertices[(index * 3) + 1] += voxel.mRelativePositionDelta.y;
        //         vMesh->mMesh->mVertices[(index * 3) + 2] += voxel.mRelativePositionDelta.z;
      }
    }
    mRenderer->UpdateMesh(handle, vMesh->mMesh->mIndices);
  }
}

void PhysicsSimulationApp::Render()
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
