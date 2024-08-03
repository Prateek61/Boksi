#include "bkpch.h"
#include "IndexBuffer.h"

#include "Boksi/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer/OpenGLBuffer.h"

namespace Boksi
{
    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {

        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            BK_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLIndexBuffer(indices, count);
        }

        BK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}