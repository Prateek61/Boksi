#include "bkpch.h"
#include "Renderer.h"
#include "Shader.h"
#include "OrthographicCamera.h"

namespace Boksi
{
	void Renderer::BeginScene()
    {
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray)
    {
        shader->Bind();
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::DispatchCompute(Ref<ComputeShader>& shader, uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ)
    {
        shader->Bind();
        RenderCommand::DispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
    }

    std::string Renderer::ReadFile(const std::string &filepath)
    {
        if (!std::filesystem::exists(filepath))
        {
            // Use your custom assert function or handle the error as needed
            BK_CORE_ASSERT(false, "File does not exist!");
            // Print the current file path of the current execution

            return "";
        }

        std::ifstream file(filepath);
        std::string result;
        std::string line;
        while (std::getline(file, line))
        {
            result += line + "\n";
        }
        return result;
    }
}; // namespace Boksi