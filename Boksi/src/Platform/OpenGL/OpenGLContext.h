#pragma once

#include "Boksi/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Boksi
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow *windowHandle);

        void Init() override;
        void SwapBuffers() override;
    private:
        GLFWwindow *m_WindowHandle;
    };
} // namespace Boksi