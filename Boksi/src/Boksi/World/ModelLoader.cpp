#include "bkpch.h"
#include "ModelLoader.h"
#include "Mesh/VoxelMesh.h"
#include "Boksi/World/Material.h"

namespace Boksi
{
    struct Vec3Comparator
    {
        bool operator()(const glm::vec3 &lhs, const glm::vec3 &rhs) const
        {
            if (lhs.x != rhs.x)
                return lhs.x < rhs.x;
            if (lhs.y != rhs.y)
                return lhs.y < rhs.y;
            return lhs.z < rhs.z;
        }
    };

    void ModelLoader::LoadModel(const std::string path, Ref<VoxelMesh> mesh, glm::uvec3 pos, int scale)
    {
        // load file from path in read mode

        // Storage an array of materials
        std::map<glm::vec3, uint8_t, Vec3Comparator> materials;

        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            BK_CORE_ERROR("Could not open file: {0}", path);
            return;
        }

        // read the file
        std::string line;
        while (std::getline(file, line))
        {
            // first line is the dimension
            if (line[0] == 'D')
            {
                std::string dim = line.substr(2);
                std::vector<std::string> dims = SplitString(dim, ' ');
                BK_CORE_ASSERT(dims.size() == 3, "Invalid dimension format");
                int x = std::stoi(dims[0]);
                int y = std::stoi(dims[1]);
                int z = std::stoi(dims[2]);

                BK_CORE_ASSERT(x > 0 && y > 0 && z > 0, "Invalid dimensions");
            }

            else
            {
                std::vector<std::string> data = SplitString(line, ' ');
                BK_CORE_ASSERT(data.size() == 6, "Invalid data format");
                int x = pos.x + std::stoi(data[0]) * scale;
                int y = pos.y + std::stoi(data[1]) * scale;
                int z = pos.z + std::stoi(data[2]) * scale;

                int r = std::stoi(data[3]);
                int g = std::stoi(data[4]);
                int b = std::stoi(data[5]);

                BK_CORE_ASSERT(x >= 0 && x < mesh->GetSize().x, "Invalid x coordinate");
                BK_CORE_ASSERT(y >= 0 && y < mesh->GetSize().y, "Invalid y coordinate");
                BK_CORE_ASSERT(z >= 0 && z < mesh->GetSize().z, "Invalid z coordinate");

                glm::uvec3 pos = {x, y, z};
                glm::vec3 color = {r / 255.0, g / 255.0, b / 255.0};

                uint8_t materialID = 0;

                // Check if the color exists in the materials map
                if (materials.find(color) == materials.end())
                {
                    // Add the color to the materials map
                    Material material;
                    // material.Color = glm::vec3(LinearToGamma(color.r), LinearToGamma(color.g), LinearToGamma(color.b));
                    material.Color = color;
                    material.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
                    material.Specular = glm::vec3(0.5f, 0.5f, 0.5f);
                    materialID = MaterialLibrary::AddMaterial(material, "LoadMaterial: " + std::to_string(MaterialLibrary::GetMaterialCount()));
                    materials[color] = materialID;
                }
                else
                {
                    materialID = materials[color];
                }

                mesh->SetVoxel(pos, materialID);
            }
        }
    }

    std::vector<std::string> ModelLoader::CreateCubeToEntity(const glm::vec3 dimensions)
    {
        std::vector<std::string> model;
        for (int x = 0; x < dimensions.x; x++)
        {
            for (int y = 0; y < dimensions.y; y++)
            {
                for (int z = 0; z < dimensions.z; z++)
                {
                    model.push_back(std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " 1");
                }
            }
        }
        return model;
    }

    std::vector<std::string> ModelLoader::LoadModelToEntity(const std::string path, int scale)
    {
        // load file from path in read mode

        std::vector<std::string> model;

        // Storage an array of materials
        std::map<glm::vec3, uint8_t, Vec3Comparator> materials;

        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            BK_CORE_ERROR("Could not open file: {0}", path);
            return model;
        }

        // read the file
        std::string line;
        while (std::getline(file, line))
        {
            // first line is the dimension
            if (line[0] == 'D')
            {
                std::string dim = line.substr(2);
                std::vector<std::string> dims = SplitString(dim, ' ');
                BK_CORE_ASSERT(dims.size() == 3, "Invalid dimension format");
                int x = std::stoi(dims[0]);
                int y = std::stoi(dims[1]);
                int z = std::stoi(dims[2]);

                BK_CORE_ASSERT(x > 0 && y > 0 && z > 0, "Invalid dimensions");

                model.push_back("D " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z));
            }

            else
            {
                std::vector<std::string> data = SplitString(line, ' ');
                BK_CORE_ASSERT(data.size() == 6, "Invalid data format");

                int x = std::stoi(data[0]) * scale;
                int y = std::stoi(data[1]) * scale;
                int z = std::stoi(data[2]) * scale;

                int r = std::stoi(data[3]);
                int g = std::stoi(data[4]);
                int b = std::stoi(data[5]);

                glm::uvec3 pos = {x, y, z};
                glm::vec3 color = {r / 255.0, g / 255.0, b / 255.0};

                uint8_t materialID = 0;

                // Check if the color exists in the materials map
                if (materials.find(color) == materials.end())
                {
                    // Add the color to the materials map
                    Material material;
                    material.Color = glm::vec3(LinearToGamma(color.r), LinearToGamma(color.g), LinearToGamma(color.b));
                    material.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
                    material.Specular = glm::vec3(0.5f, 0.5f, 0.5f);
                    materialID = MaterialLibrary::AddMaterial(material, "LoadMaterial: " + std::to_string(MaterialLibrary::GetMaterialCount()));
                    materials[color] = materialID;
                }
                else
                {
                    materialID = materials[color];
                }

                model.push_back(std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(materialID));
            }
        }

        return model;
    }

    float ModelLoader::LinearToGamma(float linear)
    {
        if (linear <= 0.0031308f)
        {
            return 12.92f * linear;
        }
        else
        {
            return 1.055f * std::pow(linear, 1.0f / 2.4f) - 0.055f;
        }
    }
}
