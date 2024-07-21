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

        BK_CORE_INFO("OpenGL Version: {0}");
        // BK_CORE_INFO("GLSL Version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
        // BK_CORE_INFO("OpenGL Vendor: {0}", glGetString(GL_VENDOR));
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}