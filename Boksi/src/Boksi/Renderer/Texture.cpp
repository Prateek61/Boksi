#include "bkpch.h"
#include "Texture.h"

#include "Boksi/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Boksi
{
	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: BK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(specification);
		}

		BK_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: BK_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}

		BK_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}