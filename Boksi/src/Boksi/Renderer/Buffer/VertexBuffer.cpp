#include "bkpch.h"
#include "VertexBuffer.h"

#include "Boksi/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer/OpenGLBuffer.h"

namespace Boksi
{
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            BK_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
        }

        BK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}