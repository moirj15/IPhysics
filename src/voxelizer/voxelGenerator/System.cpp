//
// Created by Jimmy on 2/9/2020.
//

#include "System.h"

#include "../../../imgui/imgui.h"
#include "Obj.h"
#include "Voxelizer.h"
#include "VoxelizerUI.h"
#include "tiny_obj_loader.h"

#include <GLFW/glfw3.h>
#include <Renderer/Camera.h>
#include <Renderer/Mesh.h>
#include <Renderer/RendererBackend.h>
#include <Renderer/RendererFrontend.h>
#include <Renderer/Window.h>
#include <Utils/QuickCastBuffer.h>
#include <Utils/Serialization.h>
#include <VoxelObjects/VoxelMesh.h>
#include <cstdio>
#include <glm/gtx/transform.hpp>

namespace VoxGen
{

System::System() :
    mCamera(
        glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    mWindow(Renderer::Init(1980, 1080, "Voxel Generator", false)), mUI(new VoxelizerUI()),
    /*mRenderer(new Renderer::RendererFrontend(mWindow.get(), &mCamera)),*/ mVoxelizer(
        new Voxelizer()),
    mProjectionMat(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f)),
    mCurrentMeshHandle(0), mCurrentVoxelMeshHandle(0)
{
  mUI->Init(mWindow.get());
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
    SaveVoxels();
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
    mCamera.Rotate(mouseDelta * io.DeltaTime * 10.0f);
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
#if 0
      mRenderer->RemoveMesh(mCurrentMeshHandle);
      mCurrentMeshHandle = mRenderer->RegisterMesh(mMesh.get());
#endif
      Renderer::RemoveMesh(mCurrentMeshHandle);
      mCurrentMeshHandle =
          Renderer::SubmitStaticMesh(mMesh.get(), Renderer::ShaderProgram::FlatLight);
      // ObjReader objReader;
      //       tinyobj::attrib_t attrib;
      //       std::vector<tinyobj::shape_t> shapes;
      //       std::vector<tinyobj::material_t> materials;
      //       std::string warn;
      //       std::string err;
      //       bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
      //       meshPath->c_str());
      //
      //       mMesh.reset(new Mesh());
      //       const auto &indices = shapes[0].mesh.indices;
      //       for (u64 i = 0; i < indices.size(); i++)
      //       {
      //         mMesh->mIndecies.push_back(i);
      //
      //         mMesh->mVertecies.push_back(attrib.vertices[(3 * indices[i].vertex_index)]);
      //         mMesh->mVertecies.push_back(attrib.vertices[(3 * indices[i].vertex_index) + 1]);
      //         mMesh->mVertecies.push_back(attrib.vertices[(3 * indices[i].vertex_index) + 2]);
      //
      //         mMesh->mNormals.push_back(attrib.normals[(3 * indices[i].normal_index)]);
      //         mMesh->mNormals.push_back(attrib.normals[(3 * indices[i].normal_index) + 1]);
      //         mMesh->mNormals.push_back(attrib.normals[(3 * indices[i].normal_index) + 2]);
      //       }

      //       mRenderer->RemoveMesh(mCurrentMeshHandle);
      //       mCurrentMeshHandle = mRenderer->RegisterMesh(mMesh.get());
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
    mVoxelMesh = std::make_unique<VoxObj::VoxelMesh>(mVoxelizer->Voxelize(mMesh.get()));
    // TODO: remove duplicate vertices
#if 0
    mRenderer->RemoveMesh(mCurrentVoxelMeshHandle);
    mCurrentVoxelMeshHandle = mRenderer->RegisterVoxelMesh(mVoxelMesh.get());
#endif
    Renderer::RemoveMesh(mCurrentVoxelMeshHandle);
    mCurrentVoxelMeshHandle = Renderer::SubmitVoxelMesh(*mVoxelMesh);
    //     QuickCastBuffer<f32, glm::vec3> points;
    //     for (auto &[key, voxel] : mVoxelMesh->mVoxels)
    //     {
    //       for (const auto &bezierCurve : voxel.mBezierCurves)
    //       {
    //         points.CastBufferPushBack(bezierCurve.mControlPoints);
    //       }
    //     }
    //
    //     mRenderer->DrawPoints(points);
  }
}

void System::Render()
{
  if (mVoxelMesh)
  {
    QuickCastBuffer<f32, glm::vec3> points;
    for (auto &[key, voxel] : mVoxelMesh->mVoxels)
    {
      for (const auto &bezierCurve : voxel.mBezierCurves)
      {
        points.CastBufferPushBack(bezierCurve.mControlPoints);
      }
    }
    // TODO: add point drawing
    //    mRenderer->DrawPoints(points);
  }

  if (mCurrentMeshHandle != 0)
  {
    Renderer::Draw(
        mCurrentMeshHandle,
        {ShaderData("projection", mProjectionMat), ShaderData("camera", mCamera.CalculateMatrix()),
         ShaderData("transform", glm::translate(glm::vec3(-5.0f, 0.0f, 0.0f)))},
        Renderer::DrawMode::TRIANGLES);
  }
  if (mCurrentVoxelMeshHandle != 0)
  {
    Renderer::Draw(
        mCurrentVoxelMeshHandle,
        {ShaderData("projection", mProjectionMat), ShaderData("camera", mCamera.CalculateMatrix()),
         ShaderData("transform", glm::translate(glm::vec3(-5.0f, 0.0f, 0.0f)))},
        Renderer::DrawMode::TRIANGLES);
  }
  //  mRenderer->Draw();
  mUI->Update();
  Renderer::SwapBuffers();
  //  glfwSwapBuffers(mWindow->mGLWindow);
}

void System::SaveVoxels()
{
  auto savePath = mUI->SaveClicked();
  if (savePath && mCurrentVoxelMeshHandle != 0)
  {
    Utils::Serialize(mVoxelMesh.get(), *savePath);
  }
}

} // namespace VoxGen
