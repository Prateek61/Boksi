#include "bkpch.h"

#include "Boksi/Renderer/Renderer.h"

#include "Boksi/Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Shader.h"

namespace Boksi
{
    Shader *Shader::Create(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            BK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLShader(vertexSrc, fragmentSrc);
        }

        BK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }


    ComputeShader* ComputeShader::Create(const std::string& computeSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            BK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLComputeShader(computeSrc);
        }

        BK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
