//
// Created by Jimmy on 2/9/2020.
//

#include "System.h"

#include "../renderer/RendererBackend.h"
#include "../renderer/window.h"
#include "../utils/VoxelMeshManager.h"
#include "VoxelizerUI.h"

#include <GLFW/glfw3.h>
#include <cstdio>

namespace VoxGen
{

System::System() : mWindow(Renderer::InitAPI(1980, 1080, "Voxel Generator")), mUI(new VoxelizerUI())
{
  mUI->Init(mWindow.get());
}
System::~System() = default;

void System::Run()
{

  while (!mWindow->ShouldClose())
  {
    glfwPollEvents();
    mUI->Update();
    auto meshPath = mUI->LoadMeshClicked();
    if (meshPath)
    {
      printf("have path of %s\n", meshPath->c_str());
    }
    bool generate = mUI->GenerateVoxelsClicked();
    if (generate)
    {
      printf("generating\n");
    }
    auto voxelPath = mUI->SaveClicked();
    if (voxelPath)
    {
      printf("have path of %s\n", voxelPath->c_str());
    }
    fflush(stdout);

    glfwSwapBuffers(mWindow->mGLWindow);
  }
}

} // namespace VoxGen
