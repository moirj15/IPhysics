
#include <windows.h>
#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
  VoxGen::System system(hInstance);
  system.Run();
  return 0;
}