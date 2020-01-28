#include "../renderer/renderer.h"
#include "../renderer/window.h"

#include <stdio.h>

int main(int argc, char **argv)
{
  auto *window = ren::InitAPI(1980, 1080, "Voxel Generator");
  ren::InitUI(window);

  Renderer renderer(window);

  while (!window->ShouldClose())
  {
  }

  return 0;
}