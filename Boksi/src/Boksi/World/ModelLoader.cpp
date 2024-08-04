#include "bkpch.h"
#include "ModelLoader.h"
#include "Mesh/VoxelMesh.h"

namespace Boksi
{

    std::vector<VoxelData> ModelLoader::LoadModel(const std::string &path, glm::uvec3 pos, int scale)
    {
        std::ifstream file(path);
        std::string line;
        std::vector<VoxelData> voxels;

        if (file.is_open())
        {
            while (std::getline(file, line))
            {
                std::vector<std::string> tokens = SplitString(line, ' ');
                if (tokens.size() == 6)
                {
                    glm::vec3 position = {std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2])};
                    glm::vec3 color = {std::stof(tokens[3]), std::stof(tokens[4]), std::stof(tokens[5])};

                    VoxelData voxel = {position, color};
                    voxels.push_back(voxel);
                }
            }
        }
        else
        {
            BK_CORE_ERROR("Could not open file: {0}", path);
        }

        return voxels;
    }

} // namespace Boksi
