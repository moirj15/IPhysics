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
#include <Utils/Serialization.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
    return true;
  }
  switch (msg) {
  case WM_CLOSE:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

namespace VoxGen
{

System::System(HINSTANCE hInstance) :
    mCamera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    mUI(new VoxelizerUI()), mProjectionMat(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f))
//    mCurrentMeshHandle(0), mCurrentVoxelMeshHandle(0)
{
  focus::gContext->Init(focus::RendererAPI::OpenGL, MessageHandler, hInstance);
  mWindow = focus::gContext->MakeWindow(1980, 1080);
  mUI->Init(mWindow);
}
System::~System() = default;

void System::Run()
{
  MSG msg = {};
  auto dc = GetDC(mWindow.mWindowHandle);
  while (msg.message != WM_QUIT) {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    CollectInput();
    LoadMesh();
    GenerateVoxels();
    Render();
    SaveVoxels();

    SwapBuffers(dc);
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
      //TODO: mem-leak, fix up the obj parser
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
  mUI->Update();
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
