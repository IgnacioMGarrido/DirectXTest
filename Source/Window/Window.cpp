#include "Window/Window.h"
#include <iostream>
namespace Core
{

    Window::Window(WindowSize i_size, const std::string& i_title)
        : m_size(i_size)
        , m_title(i_title)
    {

    }
    
    Window::~Window()
    {
        glfwTerminate();
    }

    void Window::Init()
    {
        if (!glfwInit())
        {
            std::cout << "Couldn't create Window\n";
            return;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


        m_window = glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), nullptr, nullptr);
    }

    void Window::Run()
    {
        glfwPollEvents();
    }

    WindowSize Window::GetSize() const
    {
        return m_size;
    }

    GLFWwindow* Window::GetWindowHandler() const
    {
        return m_window;
    }

    HWND Window::GetWindowNativeHandler() const
    {
        return glfwGetWin32Window(m_window);
    }

    bool Window::IsVisible() const
    {
        return !glfwWindowShouldClose(m_window);
    }
}
