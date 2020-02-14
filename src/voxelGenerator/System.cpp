//
// Created by Jimmy on 2/9/2020.
//

#include "System.h"

#include "../renderer/RendererBackend.h"
#include "../renderer/RendererFrontend.h"
#include "../renderer/mesh.h"
#include "../renderer/window.h"
#include "../utils/VoxelMeshManager.h"
#include "VoxelizerUI.h"
#include "obj.h"

#include <GLFW/glfw3.h>
#include <cstdio>

namespace VoxGen
{

System::System() :
    mWindow(Renderer::InitAPI(1980, 1080, "Voxel Generator")), mUI(new VoxelizerUI()),
    mRenderer(new Renderer::RendererFrontend(mWindow.get())), mCurrentMeshHandle(0)
{
  mUI->Init(mWindow.get());
}
System::~System() = default;

void System::Run()
{
  while (!mWindow->ShouldClose())
  {
    mRenderer->Clear();
    glfwPollEvents();
    auto meshPath = mUI->LoadMeshClicked();
    if (meshPath)
    {
      if (fs::exists(*meshPath))
      {
        VoxGen::ObjReader objReader;
        std::unique_ptr<Mesh> mesh(objReader.Parse(meshPath->c_str()));
        mRenderer->RemoveMesh(mCurrentMeshHandle);
        mCurrentMeshHandle = mRenderer->RegisterMesh(mesh.get());
      }
      else
      {
        // TODO: pop-up or something
      }
    }
    if (mCurrentMeshHandle != 0)
    {
      mRenderer->DrawMesh(mCurrentMeshHandle);
    }

    mRenderer->Draw();
    mUI->Update();
    glfwSwapBuffers(mWindow->mGLWindow);
  }
}

} // namespace VoxGen
