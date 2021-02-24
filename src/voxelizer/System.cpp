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
#include <glm/vec3.hpp>


System::System() :
    mCamera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    mUI(new VoxelizerUI()), mProjectionMat(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f))
//    mCurrentMeshHandle(0), mCurrentVoxelMeshHandle(0)
{
  mWindow = focus::gContext->MakeWindow(1980, 1080);
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
      mModelLoaded = true;
#if 0
      mCurrentMeshHandle = mOriginalManager.CreateMesh(std::move(*objReader.Parse(meshPath->c_str())));
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
  if (mModelLoaded && mUI->GenerateVoxelsClicked()) {
    objs::AABB aabb;
    for (auto i : mMesh.indices) {
      aabb.min = glm::min(aabb.min, mMesh.GetVertex(i));
      aabb.max = glm::max(aabb.max, mMesh.GetVertex(i));
    }
    auto params = mUI->GetParameters();
    mVoxelMesh = mVoxelizer.Voxelize(aabb, params.mVoxelSize, mMesh);
//    mVoxelizer.SetParameters(mUI->GetParameters());
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
#if 0
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
#endif

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
//    Utils::Serialize(mVoxelMesh.get(), *savePath);
  }
}

void System::DebugDrawVoxels()
{
#if 0
  if (mMesh.vertices.empty())
    return;
  static bool init = false;
  if (!init) {
    init = true;
    // find the aabb real quick
    glm::vec3 minP((float)INFINITY, (float)INFINITY, (float)INFINITY), maxP(-INFINITY, -INFINITY, -INFINITY);
    for (auto i : mMesh.indices) {
#undef min
#undef max
      minP = glm::min(minP, mMesh.GetVertex(i));
      maxP = glm::max(maxP, mMesh.GetVertex(i));
    }
    auto params = mUI->GetParameters();

    focus::ShaderBufferDescriptor inputDesc = {
        .name = "InputData",
        .slot = 0,
        .accessMode = focus::AccessMode::ReadOnly,
        .types = {focus::VarType::Vec3},
    };
    glm::ivec3 voxelDimmensions = (maxP + (-minP)) / (1.0f / params.mVoxelSize);
    struct {
      glm::vec3 min;
      float p0;
      glm::vec3 max;
      float p1;
      glm::ivec3 voxelDimmensions;
      float voxelSize;
    } cb = {.min = minP, .max = maxP, .voxelDimmensions = voxelDimmensions, .voxelSize = params.mVoxelSize};
    focus::ConstantBufferDescriptor cbDesc = {
        .name = "Constants",
        .slot = 0,
    };
    // TODO: need a buffer class
    mHullPassConstantBuffer = focus::gContext->CreateConstantBuffer((void *)&cb, 48, cbDesc);

    mHullPassInputData = focus::gContext->CreateShaderBuffer(
        (void *)mMesh.mVertices.BufferPtr(), mMesh.mVertices.BufferSizeBytes(), inputDesc);

    focus::ShaderBufferDescriptor vertexOutputDesc = {
        .name = "vertexOwner",
        .slot = 0,
        .accessMode = focus::AccessMode::WriteOnly,
        .types = {focus::VarType::Int},
    };
    focus::ShaderBufferDescriptor voxelOutputDesc = {
        .name = "existingVoxels",
        .slot = 1,
        .accessMode = focus::AccessMode::WriteOnly,
        .types = {focus::VarType::UInt},
    };

    mHullPassVertexOutput =
        focus::gContext->CreateShaderBuffer(nullptr, sizeof(f32) * mMesh.mVertices.BufferSize(), vertexOutputDesc);
    std::vector<u32> voxelPassZeroed(8 * sizeof(u32));
    // TODO: hard-coded temp
    mHullPassVoxelOutput = focus::gContext->CreateShaderBuffer(voxelPassZeroed.data(), 8 * sizeof(u32), voxelOutputDesc);
  }
  focus::ComputeState computeState = {
      .bufferHandles = {mHullPassInputData, mHullPassVertexOutput, mHullPassVoxelOutput},
      .cbHandles = {mHullPassConstantBuffer},
  };
  focus::gContext->DispatchCompute(mMesh.mVertices.BufferSize() / 3, 1, 1, mHullPass, computeState);
  focus::gContext->WaitForMemory(0);
#endif

//  s32 *voxelOutput = (s32 *)focus::gContext->MapBufferPtr(mHullPassVoxelOutput, focus::AccessMode::ReadOnly);
//  focus::gContext->UnmapBufferPtr(mHullPassVoxelOutput);
}

