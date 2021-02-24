#include <SDL.h>
#include "System.h"

int main(int argc, char **argv)
{
  setvbuf(stdout, nullptr, _IONBF, 0);
  focus::Context::Init(focus::RendererAPI::DX11);
  System system{};
  system.Run();
  return 0;
}
