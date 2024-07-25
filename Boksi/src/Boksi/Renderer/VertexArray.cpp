#include "bkpch.h"
#include "VertexArray.h"

#include "Boksi/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Boksi
{
	VertexArray *VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			BK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}

		BK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
