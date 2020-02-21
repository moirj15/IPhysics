//
// Created by Jimmy on 2/9/2020.
//

#include "System.h"

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
    mCurrentMeshHandle(0), mCurrentVoxelMeshHandle(0)
{
  mUI->Init(mWindow.get());
  mRenderer->SetProjection(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f));
}
System::~System() = default;

void System::Run()
{
  Camera camera(
      glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  while (!mWindow->ShouldClose())
  {
    // TODO: add camera movement features
    mRenderer->SetCamera(camera.CalculateMatrix());
    mRenderer->Clear();
    glfwPollEvents();
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
    // Only draw if we have a mesh
    if (mCurrentMeshHandle != 0)
    {
      mRenderer->DrawMesh(mCurrentMeshHandle);
      if (mUI->GenerateVoxelsClicked())
      {
        mVoxelizer->SetParameters(mUI->GetParameters());
        mVoxelMesh.reset(new VoxObj::VoxelMesh(mVoxelizer->Voxelize(mMesh.get())));
        mRenderer->RemoveMesh(mCurrentVoxelMeshHandle);
        mCurrentVoxelMeshHandle = mRenderer->RegisterVoxelMesh(mVoxelMesh.get());
      }
    }
    if (mCurrentVoxelMeshHandle != 0)
    {
      mRenderer->DrawMesh(mCurrentVoxelMeshHandle);
    }

    mRenderer->Draw();
    mUI->Update();
    glfwSwapBuffers(mWindow->mGLWindow);
  }
}

} // namespace VoxGen
