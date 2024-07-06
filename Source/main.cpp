#include "Window.h"

int main()
{
    Core::Window window{ {.x = 1280, .y = 720}, "D3D11 Test"};

    window.Init();

    while (window.IsVisible())
    {
        window.Run();
    }

    return 0;
}