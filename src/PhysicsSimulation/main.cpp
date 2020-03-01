
#include "System.h"

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  IPhysics::System system;
  system.Run();

  return 0;
}
