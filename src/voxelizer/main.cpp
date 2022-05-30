#include "System.h"

#include <SDL.h>

int main(int argc, char **argv)
{
    setvbuf(stdout, nullptr, _IONBF, 0);
    System system{};
    system.Run();
    return 0;
}
