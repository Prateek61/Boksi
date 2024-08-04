#pragma once
#include "bkpch.h"
#include "Mesh/VoxelMesh.h"

namespace Boksi
{
    struct VoxelData
    {
        glm::vec3 position;
        glm::vec3 color;
    };

    class ModelLoader
    {
    public:
        ModelLoader() = default;
        ~ModelLoader() = default;

        static std::vector<VoxelData> LoadModel(const std::string &path, glm::uvec3 pos, int scale);

        static std::vector<std::string> SplitString(const std::string &s, char delimiter)
        {
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(s);
            while (std::getline(tokenStream, token, delimiter))
            {
                tokens.push_back(token);
            }
            return tokens;
        }
    };

}