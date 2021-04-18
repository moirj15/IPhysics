
#include "PhysicsSimulationApp.h"

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;
  setvbuf(stdout, nullptr, _IONBF, 0);
  focus::Context::Init(focus::RendererAPI::OpenGL);

  IPhysics::PhysicsSimulationApp system;
  system.Run();

  return 0;
}
