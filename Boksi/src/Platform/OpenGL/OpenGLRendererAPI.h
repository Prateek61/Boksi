#pragma once

#include "Boksi/Renderer/RendererAPI.h"

namespace Boksi
{
    class OpenGLRendererAPI : public RendererAPI
    {

    public:
        virtual void SetClearColor(const glm::vec4 &color) override;
        virtual void Clear() override;
        virtual void DrawIndexed(const Ref<VertexArray> &vertexArray) override;
        virtual void DispatchCompute(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ) override;
        virtual void AddMemoryBarrier() override;
        void CheckForErrors() override;
    };
} // namespace Boksi