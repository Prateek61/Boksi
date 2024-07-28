#pragma once
#include "bkpch.h"

#include "Boksi/Renderer/Texture.h"

#include <glad/glad.h>

namespace Boksi
{
	class  OpenGLTexture2D final : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureSpecification& spec);
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D() override;
		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		uint32_t GetRendererID() const override { return m_RendererID; }
		const std::string& GetPath() const override { return m_Path; }
		const TextureSpecification& GetSpecification() const override { return m_Specification; }
		void SetData(void* data, uint32_t size) override;
		void Bind(uint32_t slot = 0) const override;
		bool IsLoaded() const override { return m_IsLoaded; }
		bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		TextureSpecification m_Specification;

		std::string m_Path;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}