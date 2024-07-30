#include "bkpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include "Boksi/Core/Application.h"
#include "Boksi/Core/Application.h"
#include "Boksi/Core/Application.h"
#include "Boksi/Core/Application.h"
#include "Boksi/Core/Application.h"
#include "Boksi/Core/Application.h"
#include "Boksi/Core/Application.h"
#include "Boksi/Core/Application.h"

namespace Boksi{

    OpenGLShader::OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        const GLchar *source = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        // Check for vertex shader compile errors
        int isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            int maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(vertexShader);

            BK_CORE_ERROR("{0}", infoLog.data());
            BK_CORE_ASSERT(false, "Vertex shader compilation failure!")
            return;
        }
        
        // Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        source = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(fragmentShader);
        
        // Check for fragment shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE)
        {
            int maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);

            BK_CORE_ERROR("{0}", infoLog.data());
            BK_CORE_ASSERT(false, "Fragment shader compilation failure!")
            return;
        }
        
        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.

        m_RendererID = glCreateProgram();

        glAttachShader(m_RendererID, vertexShader);
        glAttachShader(m_RendererID, fragmentShader);

        // Link our program
        glLinkProgram(m_RendererID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        int isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            int maxLength = 0;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_RendererID);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            BK_CORE_ERROR("{0}", infoLog.data());
            BK_CORE_ASSERT(false, "Shader link failure!")
            return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(m_RendererID, vertexShader);
        glDetachShader(m_RendererID, fragmentShader);

        UniformUploader.reset((ShaderUniformUploader*)new OpenGLShaderUniformUploader(m_RendererID));
    }    

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }


    void OpenGLShader::Unbind() const
    {
        glUseProgram(0); // Unbind the shader program
    }

    OpenGLShaderUniformUploader::OpenGLShaderUniformUploader(uint32_t rendererID)
		: m_RendererID(rendererID)
    {
    }

    void OpenGLShaderUniformUploader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
    {
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
    }

    void OpenGLShaderUniformUploader::UploadUniformFloat4(const std::string& name, const glm::vec4& values) const
    {
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShaderUniformUploader::UploadUniformFloat3(const std::string& name, const glm::vec3& values) const
    {
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShaderUniformUploader::UploadUniformFloat(const std::string& name, float value) const
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShaderUniformUploader::UploadUniformFloat2(const std::string& name, const glm::vec2& values) const
    {
    	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
    }

    void OpenGLShaderUniformUploader::UploadUniformInt(const std::string& name, int value) const
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShaderUniformUploader::UploadUniformInt3(const std::string &name, const glm::ivec3 &values) const
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3i(location, values.x, values.y, values.z);
    }

    OpenGLComputeShader::OpenGLComputeShader(const std::string& computeSrc)
    {
        // Create an empty compute shader handle
        GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);

        // Send the compute shader source code to GL
        const GLchar* source = computeSrc.c_str();
        glShaderSource(computeShader, 1, &source, 0);

        // Compile the compute shader
        glCompileShader(computeShader);

        // Check for compute shader compile errors
        int is_compiled = 0;
        glGetShaderiv(computeShader, GL_COMPILE_STATUS, &is_compiled);
        if (is_compiled == GL_FALSE)
        {
            int max_length = 0;
            glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &max_length);

			// The max_length includes the NULL character
			std::vector<GLchar> info_log(max_length);
			glGetShaderInfoLog(computeShader, max_length, &max_length, info_log.data());

			// We don't need the shader anymore.
			glDeleteShader(computeShader);

			BK_CORE_ERROR("{0}", info_log.data());
			BK_CORE_ASSERT(false, "Compute shader compilation failure!")
			return;
        }

        // Compute shader is successfully compiled.
        // Now time to link it into a program.
        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, computeShader);

        // Link da program
    	glLinkProgram(m_RendererID);

        // Check for compute shader link errors
        int is_linked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &is_linked);
        if (is_linked == GL_FALSE)
		{
			int max_length = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &max_length);

			// The max_length includes the NULL character
			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(m_RendererID, max_length, &max_length, info_log.data());

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(computeShader);

			BK_CORE_ERROR("{0}", info_log.data());
			BK_CORE_ASSERT(false, "Compute shader link failure!")
			return;
		}

    	// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, computeShader);

        UniformUploader.reset((ShaderUniformUploader*)new OpenGLShaderUniformUploader(m_RendererID));
    }

    OpenGLComputeShader::~OpenGLComputeShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLComputeShader::Bind() const
    {
		glUseProgram(m_RendererID);
    }

    void OpenGLComputeShader::Unbind() const
    {
        glUseProgram(0);
    }
}
