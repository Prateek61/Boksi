#pragma once
#include "bkpch.h"
#include <glm/glm.hpp>

namespace Boksi
{
    class ShaderUniformUploader
    {
    public:
        virtual ~ShaderUniformUploader() = default;

        virtual void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix) const = 0;
        virtual void UploadUniformFloat4(const std::string &name, const glm::vec4 &values) const = 0;
        virtual void UploadUniformFloat3(const std::string &name, const glm::vec3 &values) const = 0;
        virtual void UploadUniformFloat(const std::string &name, float value) const = 0;
        virtual void UploadUniformFloat2(const std::string &name, const glm::vec2 &values) const = 0;
        virtual void UploadUniformInt(const std::string &name, int value) const = 0;
        virtual void UploadUniformInt3(const std::string &name, const glm::ivec3 &values) const = 0;
    };

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Shader *Create(const std::string &vertexSrc, const std::string &fragmentSrc);

    public:
        Ref<ShaderUniformUploader> UniformUploader;
    };

    class ComputeShader
    {
    public:
        virtual ~ComputeShader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static ComputeShader *Create(const std::string &computeSrc);

    public:
        Ref<ShaderUniformUploader> UniformUploader;
    };

    class ShaderLoader
    {
    public:
        static std::string Load(const std::string& filepath);
    private:
        static std::string ReadFile(const std::string& filepath);
        static bool WriteFile(const std::string& filepath, const std::string& data);
    };
} // namespace name