#include "Window/Window.h"
#include "Renderer/Renderer.h"
int main()
{

	std::shared_ptr<Core::Window> window = std::make_shared<Core::Window>(Core::WindowSize(1280, 720), "D3D11 Test");
    window->Init();

	std::unique_ptr<Core::Renderer> renderer = std::make_unique<Core::Renderer>(window);

    while (window->IsVisible())
    {
        window->Run();

        renderer->ClearColor({1.f, 0.0f, 1.0f, 1.0f});
        renderer->Present();
    }

    return 0;
}