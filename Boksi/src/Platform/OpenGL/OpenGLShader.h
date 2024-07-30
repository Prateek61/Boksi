#pragma once
#include <string>
#include "Boksi/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Boksi
{
    class OpenGLShaderUniformUploader : ShaderUniformUploader
    {
    public:
        OpenGLShaderUniformUploader(uint32_t rendererID);
    	~OpenGLShaderUniformUploader() override = default;

        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;
        void UploadUniformFloat4(const std::string& name, const glm::vec4& values) override;
        void UploadUniformFloat3(const std::string& name, const glm::vec3& values) override;
        void UploadUniformFloat(const std::string& name, float value) override;
        void UploadUniformFloat2(const std::string& name, const glm::vec2& values) override;
        void UploadUniformInt(const std::string& name, int value) override;

    private:
        uint32_t m_RendererID;
    };

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

    private:
        uint32_t m_RendererID;
    };

    class OpenGLComputeShader : public ComputeShader
    {
    public:
        OpenGLComputeShader(const std::string& computeSrc);
        ~OpenGLComputeShader() override;

        void Bind() const override;
        void Unbind() const override;
    private:
        uint32_t m_RendererID;
    };

} // namespace name