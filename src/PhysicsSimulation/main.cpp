
#include "PhysicsSimulationSystem.h"

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  IPhysics::PhysicsSimulationSystem system;
  system.Run();

  return 0;
}
