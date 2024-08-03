#include "bkpch.h"
#include "OpenGLStorageBuffer.h"

#include "GLAD/glad.h"

namespace Boksi
{
    OpenGLStorageBuffer::OpenGLStorageBuffer()
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
        glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
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
}
