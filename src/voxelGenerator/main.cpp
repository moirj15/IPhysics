#include "../renderer/renderer.h"
#include "../renderer/window.h"
#include "obj.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

void tempVoxelizer()
{
  vg::ObjReader objReader;
  auto mesh = objReader.Parse("../testfiles/sphere.obj");
}

int main(int argc, char **argv)
{
  tempVoxelizer();
  auto *window = ren::InitAPI(1980, 1080, "Voxel Generator");
  ren::InitUI(window);

  Renderer renderer(window);
  renderer.SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

  while (!window->ShouldClose())
  {
    glfwPollEvents();
    renderer.ClearScreen();
    renderer.UpdateScreen();
  }

  return 0;
}