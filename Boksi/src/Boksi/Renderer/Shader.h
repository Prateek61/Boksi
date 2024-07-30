#pragma once
#include "bkpch.h"
#include <glm/glm.hpp>

namespace Boksi
{
    class ShaderUniformUploader
    {
    public:
        virtual ~ShaderUniformUploader() = default;

        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;
        virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values) = 0;
        virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& values) = 0;
        virtual void UploadUniformFloat(const std::string& name, float value) = 0;
        virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& values) = 0;
        virtual void UploadUniformInt(const std::string& name, int value) = 0;
    };

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Shader *Create(const std::string &vertexSrc, const std::string &fragmentSrc);

    public:
        Scope<ShaderUniformUploader> UniformUploader;
    };

    class ComputeShader
    {
    public:
        virtual ~ComputeShader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static ComputeShader* Create(const std::string& computeSrc);

    public:
        Scope<ShaderUniformUploader> UniformUploader;
    };
} // namespace name