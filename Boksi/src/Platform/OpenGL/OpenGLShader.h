#pragma once
#include <string>
#include "Boksi/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Boksi
{

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

         void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix);
         void UploadUniformFloat4(const std::string &name, const glm::vec4 &values);
       



    private:
        uint32_t m_RendererID;
    };

} // namespace name