#include "bkpch.h"

#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Boksi
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    const BufferLayout& OpenGLVertexBuffer::GetLayout() const
    {
        return m_Layout;
    }

    void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
    {
        m_Layout = layout;
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count)
        : m_Count(count)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    OpenGLStorageBuffer::OpenGLStorageBuffer(uint32_t size)
	    : m_Size(size)
    {
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLStorageBuffer::~OpenGLStorageBuffer()
    {
		glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLStorageBuffer::Bind(uint32_t slot) const
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_RendererID);
    }

    void OpenGLStorageBuffer::Unbind() const
    {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void OpenGLStorageBuffer::SetData(void* data, uint32_t size)
    {
        glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }

} // namespace Boksi