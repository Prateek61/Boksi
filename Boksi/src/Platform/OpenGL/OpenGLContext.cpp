#include "bkpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"

namespace Boksi
{

    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
        : m_WindowHandle(windowHandle)
    {
        BK_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        BK_CORE_ASSERT(status, "Failed to initialize Glad!");


        std::string version(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
        BK_CORE_INFO("OpenGL Version: {0}", version);
        std::string vendor(reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
        BK_CORE_INFO("OpenGL Vendor: {0}", vendor);
        std::string renderer(reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
        BK_CORE_INFO("OpenGL Renderer: {0}", renderer);
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}