//
// Created by Jimmy on 2/9/2020.
//

#include "System.h"

#include "../../imgui/imgui.h"
#include "../Input/InputManager.h"
#include "../renderer/RendererBackend.h"
#include "../renderer/RendererFrontend.h"
#include "../renderer/camera.h"
#include "../renderer/mesh.h"
#include "../renderer/window.h"
#include "../utils/VoxelMeshManager.h"
#include "../voxelObjects/VoxelMesh.h"
#include "Voxelizer.h"
#include "VoxelizerUI.h"
#include "obj.h"

#include <GLFW/glfw3.h>
#include <cstdio>

namespace VoxGen
{

System::System() :
    mWindow(Renderer::InitAPI(1980, 1080, "Voxel Generator")), mUI(new VoxelizerUI()),
    mRenderer(new Renderer::RendererFrontend(mWindow.get())), mVoxelizer(new Voxelizer()),
    mCamera(
        glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    mCurrentMeshHandle(0), mCurrentVoxelMeshHandle(0)
{
  mUI->Init(mWindow.get());
  mRenderer->SetProjection(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f));
  Input::Init(mWindow->mGLWindow);
}
System::~System() = default;

void System::Run()
{
  while (!mWindow->ShouldClose())
  {
    CollectInput();
    LoadMesh();
    GenerateVoxels();
    Render();
  }
}

// Private;
void System::CollectInput()
{
  glfwPollEvents();
  auto &io = ImGui::GetIO();
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
  if (!io.WantCaptureMouse && io.MouseDown[0])
  {
    f32 screenWidth = f32(mWindow->GetWidth());
    f32 screenHeight = f32(mWindow->GetHeight());
    glm::vec2 mouseDelta(
        (screenWidth / 2.0f) - io.MousePos.x, (screenHeight / 2.0f) - io.MousePos.y);
    // glm::vec2 mouseDelta(io.MouseDelta.x, io.MouseDelta.y);
    mCamera.Rotate(mouseDelta);
  }
}

void System::LoadMesh()
{
  auto meshPath = mUI->LoadMeshClicked();
  if (meshPath)
  {
    // Load our mesh if we got a good path
    if (fs::exists(*meshPath))
    {
      ObjReader objReader;
      mMesh.reset(objReader.Parse(meshPath->c_str()));
      mRenderer->RemoveMesh(mCurrentMeshHandle);
      mCurrentMeshHandle = mRenderer->RegisterMesh(mMesh.get());
    }
    else
    {
      // TODO: pop-up or something
    }
  }
}

void System::GenerateVoxels()
{
  if (mCurrentMeshHandle != 0 && mUI->GenerateVoxelsClicked())
  {
    mVoxelizer->SetParameters(mUI->GetParameters());
    mVoxelMesh.reset(new VoxObj::VoxelMesh(mVoxelizer->Voxelize(mMesh.get())));
    mRenderer->RemoveMesh(mCurrentVoxelMeshHandle);
    mCurrentVoxelMeshHandle = mRenderer->RegisterVoxelMesh(mVoxelMesh.get());
  }
}

void System::Render()
{
  mRenderer->SetCamera(mCamera.CalculateMatrix());
  mRenderer->Clear();

  if (mCurrentMeshHandle != 0)
  {
    mRenderer->DrawMesh(mCurrentMeshHandle);
  }
  if (mCurrentVoxelMeshHandle != 0)
  {
    mRenderer->DrawMesh(mCurrentVoxelMeshHandle);
  }
  mRenderer->Draw();
  mUI->Update();
  glfwSwapBuffers(mWindow->mGLWindow);
}
} // namespace VoxGen
