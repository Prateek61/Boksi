#include "bkpch.h"
#include "Renderer.h"

namespace Boksi
{

    void Renderer::BeginScene()
    {
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<VertexArray> &vertexArray)
    {
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);

    }

}; // namespace Boksi