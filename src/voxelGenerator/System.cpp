//
// Created by Jimmy on 2/9/2020.
//

#include "System.h"

#include "../renderer/renderer.h"
#include "../renderer/window.h"
#include "VoxelizerUI.h"

#include <GLFW/glfw3.h>

namespace VoxGen
{

System::System() : mWindow(ren::InitAPI(1980, 1080, "Voxel Generator")), mUI(new VoxelizerUI())
{
  mUI->Init(mWindow.get());
}
System::~System()
{
}

void System::Run()
{

  while (!mWindow->ShouldClose())
  {
    glfwPollEvents();
    mUI->Update();

    glfwSwapBuffers(mWindow->mGLWindow);
  }
}

} // namespace VoxGen
