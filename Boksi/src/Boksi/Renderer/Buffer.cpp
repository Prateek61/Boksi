#include "bkpch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Boksi
{

    // --------------------------------------------------------------------------------------------
    // Vertex Buffer
    // --------------------------------------------------------------------------------------------
    VertexBuffer *VertexBuffer::Create(float *vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            BK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
        }

        BK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    // --------------------------------------------------------------------------------------------
    // Index Buffer
    // --------------------------------------------------------------------------------------------

    IndexBuffer *IndexBuffer::Create(uint32_t *indices, uint32_t count)
    {

        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            BK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(indices, count);
        }

        BK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}