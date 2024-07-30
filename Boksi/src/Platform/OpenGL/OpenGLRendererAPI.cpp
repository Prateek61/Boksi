#include "bkpch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Boksi
{

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray)
    {
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::DispatchCompute(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ)
    {
    	glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
    }

    void OpenGLRendererAPI::AddMemoryBarrier()
    {
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }

    void OpenGLRendererAPI::CheckForErrors()
    {
    	GLenum error;
		while ((error = glGetError()) != GL_NO_ERROR)
		{
			BK_CORE_ERROR("OpenGL Error: {0}", error);
		}
    }

} // namespace Boksi