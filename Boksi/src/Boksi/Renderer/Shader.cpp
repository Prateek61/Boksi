#include "bkpch.h"

#include "Boksi/Renderer/Renderer.h"

#include "Boksi/Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Shader.h"

#include <regex>

namespace Boksi
{
    Shader *Shader::Create(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            BK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLShader(vertexSrc, fragmentSrc);
        }

        BK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }


    ComputeShader* ComputeShader::Create(const std::string& computeSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            BK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLComputeShader(computeSrc);
        }

        BK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    std::string FolderPath(const std::string& filepath)
    {
        // Find the last occurrence of the directory separator
        size_t pos = filepath.find_last_of("/\\");
        if (pos != std::string::npos)
        {
            // Return the substring from the start to the last directory separator
            return filepath.substr(0, pos);
        }
        // If no directory separator is found, return an empty string or some default value
        return "";
    }

    std::string FileName(const std::string& filepath)
	{
		// Find the last occurrence of the directory separator
		size_t pos = filepath.find_last_of("/\\");
		if (pos != std::string::npos)
		{
			// Return the substring from the last directory separator to the end
			return filepath.substr(pos + 1);
		}
		// If no directory separator is found, return the whole string
		return filepath;
	}

    std::string ShaderLoader::Load(const std::string& filepath)
    {
        std::string result;

        const std::string folder_path = FolderPath(filepath);
        const std::string file_name = FileName(filepath);

        result = ReadFile(filepath);

        // Look for #include "file" in the shader source and replace it with the contents of the file
        std::regex include_regex(R"(#include\s+\"([^\"]+)\")");
        std::smatch match;
        std::string processed_result = result;

        while (std::regex_search(processed_result, match, include_regex))
        {
            std::string include_filepath = folder_path + "/" + match[1].str();
            std::string include_content = ReadFile(include_filepath);
            processed_result.replace(match.position(0), match.length(0), include_content);
        }

        // Save the result to file_name + ".loaded"
        WriteFile(folder_path + "/" + file_name + ".loaded", processed_result);

        return processed_result;
    }

    std::string ShaderLoader::ReadFile(const std::string& filepath)
    {
        if (!std::filesystem::exists(filepath))
        {
            BK_CORE_ASSERT(false, "File does not exist!");
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

    bool ShaderLoader::WriteFile(const std::string& filepath, const std::string& data)
    {
        std::ofstream outfile(filepath);

        // Check if the file stream is open and ready
        if (!outfile.is_open())
        {
	        BK_CORE_ASSERT(false, "Could not open file for writing!");
			return false;
        }

        // Write the data to the file
        outfile << data;

        // Check if the write operation was successful
        if (!outfile.good())
        {
            BK_CORE_ASSERT(false, "Error occurred at writing time!");
        }

        // Close the file stream
        outfile.close();

        return true;
    }

}
