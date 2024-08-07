#include "bkpch.h"
#include "Mesh/VoxelMesh.h"

namespace Boksi
{

    class ModelLoader
    {
    public:
        ModelLoader() = default;
        ~ModelLoader() = default;

        static void LoadModel(const std::string path, Ref<VoxelMesh> mesh, glm::uvec3 pos = {0, 0, 0}, int scale = 1);
        static float LinearToGamma(float linear);
        static std::vector<std::string> LoadModelToEntity(const std::string path, int scale);
        static std::vector<std::string> CreateCubeToEntity(const glm::vec3 dimensions);
        static void SaveMeshToFile(const std::string path, Ref<VoxelMesh> mesh);

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