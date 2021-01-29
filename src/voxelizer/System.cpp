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
#include <IShader.hpp>
#include <Renderer/Window.h>
#include <SDL.h>
#include <Utils/Serialization.h>
#include <glm/glm.hpp>

namespace VoxGen
{

System::System() :
    mCamera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    mUI(new VoxelizerUI()), mProjectionMat(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f))
//    mCurrentMeshHandle(0), mCurrentVoxelMeshHandle(0)
{
  focus::Context::Init(focus::RendererAPI::OpenGL);
  mWindow = focus::gContext->MakeWindow(1980, 1080);
  mUI->Init(mWindow);
  mHullPass = focus::gContext->CreateComputeShaderFromSource("HullPass", utils::ReadEntireFileAsString("shaders/HullPass.comp"));
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
    CollectInput();
    LoadMesh();
    GenerateVoxels();
    DebugDrawVoxels();
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
      mMesh = *objReader.Parse(meshPath->c_str());
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
void System::DebugDrawVoxels()
{
  if (mMesh.mVertices.IsEmpty()) return;
  static bool init = false;
  if (!init) {
    init = true;
    // find the aabb real quick
    glm::vec3 minP((float)INFINITY, (float)INFINITY, (float)INFINITY), maxP(-INFINITY, -INFINITY, -INFINITY);
    for (auto i : mMesh.mIndices) {
#undef min
#undef max
      minP = glm::min(minP, mMesh.mVertices.AccessCastBuffer(i));
      maxP = glm::max(maxP, mMesh.mVertices.AccessCastBuffer(i));
    }
    auto params = mUI->GetParameters();
    focus::ShaderBufferDescriptor inputDesc = {
        .mName = "InputData",
        .mSlot = 1,
    };
    glm::vec3 aabb[] = {minP, maxP};
    focus::ConstantBufferDescriptor cbDesc = {
        .mName = "Constants",
        .mSlot = 0,
    };
    mHullPassConstantBuffer = focus::gContext->CreateConstantBuffer((void *)aabb, 32, cbDesc);

    glm::ivec3 dimmensions((s32)(1.0 / params.mVoxelSize));
    u8 *data = new u8[sizeof(glm::ivec3) + mMesh.mVertices.GetBuffer().size() * sizeof(f32)];
    memcpy(data, (void *)&dimmensions, sizeof(dimmensions));
    memcpy(data + sizeof(u32) + sizeof(dimmensions), mMesh.mVertices.GetBuffer().data(), mMesh.mVertices.GetBuffer().size() * sizeof(f32));
    mHullPassInputData = focus::gContext->CreateShaderBuffer(
        (void *)data, sizeof(glm::ivec3) + mMesh.mVertices.BufferSizeBytes(), inputDesc);

    focus::ShaderBufferDescriptor vertexOutputDesc = {
        .mName = "VertexOutput",
        .mSlot = 2,
    };
    focus::ShaderBufferDescriptor voxelOutputDesc = {
        .mName = "VoxelOutput",
        .mSlot = 3,
    };

    u8 *tmp = new u8[sizeof(f32) * mMesh.mVertices.BufferSize()]();
    mHullPassVertexOutput =
        focus::gContext->CreateShaderBuffer(tmp, sizeof(f32) * mMesh.mVertices.BufferSize(), vertexOutputDesc);
    u8 *tmp2 = new u8[dimmensions.x * dimmensions.y * dimmensions.z * sizeof(u32)]();
    mHullPassVoxelOutput =
        focus::gContext->CreateShaderBuffer(tmp2, dimmensions.x * dimmensions.y * dimmensions.z * sizeof(u32), voxelOutputDesc);
  }
  focus::ComputeState computeState = {
      .bufferHandles = {mHullPassInputData, mHullPassVertexOutput, mHullPassVoxelOutput},
      .cbHandles = {mHullPassConstantBuffer},
  };
  focus::gContext->DispatchCompute(mMesh.mVertices.BufferSize() / 3, 1, 1, mHullPass, computeState);
  focus::gContext->WaitForMemory(0);

  u32 *voxelOutput = (u32 *)focus::gContext->MapBufferPtr(mHullPassVoxelOutput, focus::AccessMode::ReadOnly);
  focus::gContext->UnmapBufferPtr(mHullPassVoxelOutput);
}

} // namespace VoxGen
