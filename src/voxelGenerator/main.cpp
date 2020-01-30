#include "../renderer/camera.h"
#include "../renderer/mesh.h"
#include "../renderer/renderer.h"
#include "../renderer/window.h"
#include "obj.h"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <memory>

void tempVoxelizer()
{
  vg::ObjReader objReader;
  std::unique_ptr<Mesh> mesh(objReader.Parse("../testfiles/sphere.obj"));
}

int main(int argc, char **argv)
{
  tempVoxelizer();
  auto *window = ren::InitAPI(1980, 1080, "Voxel Generator");
  ren::InitUI(window);

  Renderer renderer(window);
  renderer.SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

  Camera camera(
      glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  while (!window->ShouldClose())
  {
    glfwPollEvents();
    renderer.ClearScreen();
    renderer.UpdateScreen();
  }

  return 0;
}