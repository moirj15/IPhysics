//
// Created by Jimmy on 2/9/2020.
//

#include "System.h"

#include "../third_party/imgui/backends/imgui_impl_win32.h"
#include "Obj.h"
#include "VoxelizerUI.h"
#include "tiny_obj_loader.h"

#include <Context.hpp>
#include <GLFW/glfw3.h>
#include <Renderer/Window.h>
#include <SDL.h>
#include <Utils/Serialization.h>

namespace VoxGen
{

System::System() :
    mCamera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    mUI(new VoxelizerUI()), mProjectionMat(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f))
//    mCurrentMeshHandle(0), mCurrentVoxelMeshHandle(0)
{
  focus::gContext->Init(focus::RendererAPI::OpenGL);
  mWindow = focus::gContext->MakeWindow(1980, 1080);
  mUI->Init(mWindow);
}
System::~System() = default;

void System::Run()
{
  bool keepWindowOpen = true;
  SDL_Event e;
  while (keepWindowOpen) {
    while (SDL_PollEvent(&e) > 0) {
      if (e.type == SDL_QUIT) {
        return;
      }
    }
    CollectInput();
    LoadMesh();
    GenerateVoxels();
    Render();
    SaveVoxels();

    focus::gContext->SwapBuffers(mWindow);
  }
}

// Private;
void System::CollectInput()
{
#if 0
  glfwPollEvents();
#endif
  auto &io = ImGui::GetIO();
  f32 boost = 1.0f;
  if (io.KeysDown[GLFW_KEY_LEFT_SHIFT] && !io.WantCaptureKeyboard) {
    boost = 5.0f;
  }
  if (io.KeysDown[GLFW_KEY_W] && !io.WantCaptureKeyboard) {
    mCamera.Move(glm::vec3(0.0f, 0.0f, 1.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_S] && !io.WantCaptureKeyboard) {
    mCamera.Move(glm::vec3(0.0f, 0.0f, -1.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_A] && !io.WantCaptureKeyboard) {
    mCamera.Move(glm::vec3(-1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_D] && !io.WantCaptureKeyboard) {
    mCamera.Move(glm::vec3(1.0f, 0.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_E] && !io.WantCaptureKeyboard) {
    mCamera.Move(glm::vec3(0.0f, 1.0f, 0.0f) * boost * io.DeltaTime);
  }
  if (io.KeysDown[GLFW_KEY_Q] && !io.WantCaptureKeyboard) {
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
      mCurrentMeshHandle = mOriginalManager.CreateMesh(std::move(*objReader.Parse(meshPath->c_str())));
#if 0
      Renderer::RemoveMesh(mCurrentMeshHandle);
      mCurrentMeshHandle =
          Renderer::SubmitStaticMesh(mMesh.get(), Renderer::ShaderProgram::UniformColor);
#endif
    } else {
      // TODO: pop-up or something
    }
  }
}

void System::GenerateVoxels()
{
  // TODO: compute shader voxel generation
  if (mCurrentMeshHandle != 0 && mUI->GenerateVoxelsClicked()) {
    mVoxelizer.SetParameters(mUI->GetParameters());
    //    mVoxelMesh = std::make_unique<VoxObj::VoxelMesh>(mVoxelizer.Voxelize(mMesh.get()));

#if 0
    Renderer::RemoveMesh(mCurrentVoxelMeshHandle);
    mCurrentVoxelMeshHandle = Renderer::SubmitVoxelMesh(*mVoxelMesh);
#endif
  }
}

void System::Render()
{
  focus::gContext->Clear();
  if (mVoxelMesh) {
    QuickCastBuffer<f32, glm::vec3> points;
    for (auto &[key, voxel] : mVoxelMesh->mVoxels) {
      for (const auto &bezierCurve : voxel.mBezierCurves) {
        points.CastBufferPushBack(bezierCurve.mControlPoints);
      }
    }
    // TODO: add point drawing
    //    mRenderer->DrawPoints(points);
  }

  if (mCurrentMeshHandle != 0) {
#if 0
    Renderer::Draw(
        mCurrentMeshHandle,
        {ShaderData("projection", mProjectionMat), ShaderData("camera", mCamera.CalculateMatrix()),
         ShaderData("transform", glm::translate(glm::vec3(-5.0f, 0.0f, 0.0f)))},
        Renderer::DrawMode::TRIANGLES);
#endif
  }
  if (mCurrentVoxelMeshHandle != 0) {
#if 0
    Renderer::Draw(
        mCurrentVoxelMeshHandle,
        {ShaderData("projection", mProjectionMat), ShaderData("camera", mCamera.CalculateMatrix()),
         ShaderData("transform", glm::translate(glm::vec3(5.0f, 0.0f, 0.0f)))},
        Renderer::DrawMode::TRIANGLES);
#endif
  }
  //  mRenderer->Draw();
  mUI->Update(mWindow);
#if 0
  Renderer::SwapBuffers();
#endif
  //  glfwSwapBuffers(mWindow->mGLWindow);
}

void System::SaveVoxels()
{
  auto savePath = mUI->SaveClicked();
  if (savePath && mCurrentVoxelMeshHandle != 0) {
    Utils::Serialize(mVoxelMesh.get(), *savePath);
  }
}

} // namespace VoxGen
