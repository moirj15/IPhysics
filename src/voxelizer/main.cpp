
#include "System.h"

int main(int argc, char **argv)
{
  setvbuf(stdout, NULL, _IONBF, 0);
  VoxGen::System system{};
  system.Run();
  return 0;
}
