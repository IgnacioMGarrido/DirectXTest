#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <string>

namespace Core
{
struct WindowSize
{
    uint32_t x;
    uint32_t y;
};

class Window
{
public:
    Window(WindowSize i_size, const std::string& i_title);
    ~Window();

    void Init();
    void Run();
    WindowSize GetSize() const;

    GLFWwindow* GetWindowHandler() const;
    HWND GetWindowNativeHandler() const;

    bool IsVisible() const;
private:
    GLFWwindow* m_window;
    WindowSize m_size;
    std::string m_title;
};
}
