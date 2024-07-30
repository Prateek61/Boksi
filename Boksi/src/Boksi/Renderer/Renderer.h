#pragma once
#include "RenderCommand.h"
#include "Shader.h"
#include "OrthographicCamera.h"

namespace Boksi
{

    class Renderer
    {

    public:
        static void BeginScene();
        static void EndScene();

        static void Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray);
        static void DispatchCompute(Ref<ComputeShader>& shader, uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ);

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

        static std::string ReadFile(const std::string &filepath);
    };

}; // namespace Boksi