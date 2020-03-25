
#include "PhysicsSimulationSystem.h"

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  IPhysics::PhysicsSimulationApp system;
  system.Run();

  return 0;
}
