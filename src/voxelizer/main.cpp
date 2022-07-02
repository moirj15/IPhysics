#include "HeadSystem.h"

#include <SDL.h>

int main(int argc, char **argv)
{
    setvbuf(stdout, nullptr, _IONBF, 0);
    HeadSystem system{};
    system.Run();
    return 0;
}
