#include "bkpch.h"
#include "StorageBuffer.h"

#include "Boksi/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer/OpenGLBuffer.h"

namespace Boksi
{
    StorageBuffer* StorageBuffer::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            BK_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLStorageBuffer();
        }

        BK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}