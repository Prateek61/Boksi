#pragma once
#include "RenderCommand.h"
#include "Shader.h"
#include "OrthographicCamera.h"

namespace Boksi
{

    class Renderer
    {

    public:
        static void BeginScene(const OrthographicCamera &camera);
        static void EndScene();

        static void Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

        static std::string ReadFile(const std::string &filepath);

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        static SceneData *m_SceneData;
    };

}; // namespace Boksi