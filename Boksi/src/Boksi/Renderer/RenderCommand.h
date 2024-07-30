#pragma once

#include "VertexArray.h"
#include "RendererAPI.h"

namespace Boksi
{
    class RenderCommand
    {
    public:
        inline static void SetClearColor(const glm::vec4 &color)
        {
            s_RendererAPI->SetClearColor(color);
        }

        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }

        inline static void DrawIndexed(const Ref<VertexArray> &vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }

        inline static void DispatchCompute(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ)
		{
			s_RendererAPI->DispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
            s_RendererAPI->AddMemoryBarrier();
		}

        inline static void CheckForErrors()
		{
			s_RendererAPI->CheckForErrors();
		}

    private:
        static RendererAPI *s_RendererAPI;
    };
} // namespace Boksi