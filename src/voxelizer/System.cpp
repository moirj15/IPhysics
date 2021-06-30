#include "System.h"

#include "../third_party/imgui/backends/imgui_impl_sdl.h"
#include "../third_party/imgui/backends/imgui_impl_win32.h"
#include "FileUtils.hpp"
#include "Obj.h"
#include "VoxelizerParameters.h"
#include "VoxelizerUI.h"
#include "tiny_obj_loader.h"

#include <Common.h>
#include <Context.hpp>
#include <GLFW/glfw3.h>
#include <Renderer/Window.h>
#include <SDL.h>
#include <Utils/Serialization.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

System::System() :
    mCamera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    mWindow(focus::gContext->MakeWindow(1980, 1080)), mUI(new VoxelizerUI()),
    mProjectionMat(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f)), mRenderer(&mMeshManager)
//    mCurrentMeshHandle(0), mCurrentVoxelMeshHandle(0)
{
  mUI->Init(mWindow);
  //  mHullPass = focus::gContext->CreateComputeShaderFromSource(
  //      "HullPass", utils::ReadEntireFileAsString("shaders/HullPassCS.hlsl"));
}
System::~System() = default;

void System::Run()
{
  bool keepWindowOpen = true;
  SDL_Event e;
  while (keepWindowOpen) {
    while (SDL_PollEvent(&e) > 0) {
      // TODO: put into UI code
      ImGui_ImplSDL2_ProcessEvent(&e);
      if (e.type == SDL_QUIT) {
        return;
      }
    }
    SDL_PumpEvents();
    CollectInput(e);
    LoadMesh();
    GenerateVoxels();
    Render();
    SaveVoxels();
  }
}

// Private;
void System::CollectInput(const SDL_Event &e)
{
#if 0
  glfwPollEvents();
#endif
  auto &io = ImGui::GetIO();
  auto *keys = SDL_GetKeyboardState(nullptr);
  if (io.WantCaptureKeyboard) {
    return;
  }
  f32 boost = 1.0f;
  auto key = e.key.keysym.sym;
  if ((SDL_GetModState() & SDLK_LSHIFT) && !io.WantCaptureKeyboard) {
    boost = 10.0f;
  }
  if (keys[SDL_SCANCODE_W]) {
    mCamera.Move(glm::vec3(0.0f, 0.0f, 1.0f) * boost * io.DeltaTime);
  }
  if (keys[SDL_SCANCODE_S]) {
    mCamera.Move(glm::vec3(0.0f, 0.0f, -1.0f) * boost * io.DeltaTime);
  }
  if (keys[SDL_SCANCODE_A]) {
    mCamera.Move(glm::vec3(-1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (keys[SDL_SCANCODE_D]) {
    mCamera.Move(glm::vec3(1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (keys[SDL_SCANCODE_E]) {
    mCamera.Move(glm::vec3(0.0f, 1.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (keys[SDL_SCANCODE_Q]) {
    mCamera.Move(glm::vec3(0.0f, -1.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (!io.WantCaptureMouse && io.MouseDown[0]) {
    f32 screenWidth = f32(mWindow.mWidth);
    f32 screenHeight = f32(mWindow.mHeight);
    glm::vec2 mouseDelta((screenWidth / 2.0f) - io.MousePos.x, (screenHeight / 2.0f) - io.MousePos.y);
    mCamera.Rotate(mouseDelta * io.DeltaTime * 10.0f);
  }
}

void System::LoadMesh()
{
  auto meshPath = mUI->LoadMeshClicked();
  if (meshPath) {
    // Load our mesh if we got a good path
    if (fs::exists(*meshPath)) {
      ObjReader objReader;
      // TODO: mem-leak, fix up the obj parser
      mMesh = *objReader.Parse(meshPath->c_str());
      mCurrentMesh = mMeshManager.AddMesh(mMesh);
      mRenderer.LoadMesh(mCurrentMesh);

      mModelLoaded = true;
      mVoxelized = false;
    } else {
      // TODO: pop-up or something
    }
  }
}

void System::GenerateVoxels()
{
  if (mModelLoaded && mUI->GenerateVoxelsClicked()) {
    auto params = mUI->GetParameters();
    mVoxelizer.SetParameters(mUI->GetParameters());
    mVoxelMesh = mVoxelizer.Voxelize(mMesh);
    mMeshManager.AddVoxelMesh(mVoxelMesh, mCurrentMesh);
    mVoxelized = true;
    mRenderer.LoadDebugMesh(mCurrentMesh);
  }
}

void System::Render()
{
  mRenderer.ClearScreen();
  if (mCurrentMesh != 0) {
    mRenderer.DrawMesh(mCurrentMesh, mCamera, glm::identity<glm::mat4>());
  }
  if (mVoxelized /*&& mEnableDebugDraw*/) {
    mRenderer.DrawDebugVoxels(mCurrentMesh, mCamera, glm::identity<glm::mat4>());
  }

  mUI->Update(mWindow);
  mRenderer.UpdateScreen(mWindow);
}

void System::SaveVoxels()
{
  auto savePath = mUI->SaveClicked();
  if (savePath && mVoxelized) {
    auto params = mUI->GetParameters();
    shared::Serialize(*mMeshManager.GetMesh(mCurrentMesh), *mMeshManager.GetVoxelMesh(mCurrentMesh), *savePath, "todo",
        params.mVoxelSize, params.mHollow);
  }
}
