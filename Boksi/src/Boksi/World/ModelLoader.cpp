#include "bkpch.h"
#include "ModelLoader.h"
#include "Mesh/VoxelMesh.h"
#include "Boksi/World/Material.h"
#include <glm/gtx/quaternion.hpp>

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

    void ModelLoader::LoadModel(const std::string path, Ref<VoxelMesh> mesh, glm::vec3 pos, glm::vec3 direction, int scale, bool del)
    {
        // Load file from path in read mode
        std::map<glm::vec3, MATERIAL_ID_TYPE, Vec3Comparator> materials;
        std::vector<std::string> model;
        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            BK_CORE_ERROR("Could not open file: {0}", path);
            return;
        }

        glm::vec3 low = glm::vec3(std::numeric_limits<int>::max());
        glm::vec3 high = glm::vec3(std::numeric_limits<int>::min());

        // Read the file
        std::string line;
        while (std::getline(file, line))
        {
            // First line is the dimension
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
                int x = std::stoi(data[0]) * scale;
                int y = std::stoi(data[1]) * scale;
                int z = std::stoi(data[2]) * scale;

                int r = std::stoi(data[3]);
                int g = std::stoi(data[4]);
                int b = std::stoi(data[5]);

                BK_CORE_ASSERT(x >= 0 && x < mesh->GetSize().x, "Invalid x coordinate");
                BK_CORE_ASSERT(y >= 0 && y < mesh->GetSize().y, "Invalid y coordinate");
                BK_CORE_ASSERT(z >= 0 && z < mesh->GetSize().z, "Invalid z coordinate");

                if (x < low.x)
                    low.x = x;
                if (y < low.y)
                    low.y = y;
                if (z < low.z)
                    low.z = z;

                if (x > high.x)
                    high.x = x;
                if (y > high.y)
                    high.y = y;
                if (z > high.z)
                    high.z = z;

                glm::uvec3 pos = {x, y, z};
                glm::vec3 color = {r / 255.0f, g / 255.0f, b / 255.0f};

                MATERIAL_ID_TYPE materialID = 0;
                if (materials.find(color) == materials.end() && !del)
                {
                    Material material;
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

                model.push_back(std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(materialID));
            }
        }

        // Create a quaternion from the direction vector
        glm::quat rotation = glm::rotation(glm::vec3(0, 0, 1), direction);

        for (const std::string &data : model)
        {
            std::vector<std::string> tokens = SplitString(data, ' ');
            BK_CORE_ASSERT(tokens.size() == 4, "Invalid data format");

            int x = std::stoi(tokens[0]);
            int y = std::stoi(tokens[1]);
            int z = std::stoi(tokens[2]);
            MATERIAL_ID_TYPE materialID = std::stoi(tokens[3]);

            glm::vec3 drawPos = glm::vec3(x, y, z) - low;

            // Rotate the position based on direction quaternion
            drawPos = glm::rotate(rotation, drawPos) + pos;

            if (drawPos.x < 0 || drawPos.x >= mesh->GetSize().x || drawPos.y < 0 || drawPos.y >= mesh->GetSize().y || drawPos.z < 0 || drawPos.z >= mesh->GetSize().z)
            {
                BK_CORE_ERROR("Invalid position: {0} {1} {2}", drawPos.x, drawPos.y, drawPos.z);
                return;
            }

            if (del)
            {
                mesh->SetVoxel(drawPos, EMPTY_VOXEL);
            }
            else
            {
                mesh->SetVoxel(drawPos, materialID);
            }
        }

        BK_INFO("Model loaded at: {0} {1} {2}", pos.x, pos.y, pos.z);
        mesh->MeshChanged = true;
    }

    void ModelLoader::DrawCube(Ref<VoxelMesh> mesh, glm::vec3 pos, glm::vec3 dimensions, MATERIAL_ID_TYPE materialID)
    {
        for (int x = 0; x < dimensions.x; x++)
        {
            for (int y = 0; y < dimensions.y; y++)
            {
                for (int z = 0; z < dimensions.z; z++)
                {
                    mesh->SetVoxel(glm::vec3(x, y, z) + pos, materialID);
                }
            }
        }
        mesh->MeshChanged = true;
        BK_INFO("Cube drawn at: {0} {1} {2}", pos.x, pos.y, pos.z);
    }

    void ModelLoader::DrawSphere(Ref<VoxelMesh> mesh, glm::vec3 pos, float radius, MATERIAL_ID_TYPE materialID)
    {

        for (int x = -radius; x <= radius; x++)
        {
            for (int y = -radius; y <= radius; y++)
            {
                for (int z = -radius; z <= radius; z++)
                {
                    if (glm::length(glm::vec3(x, y, z)) <= radius)
                    {
                        mesh->SetVoxel(glm::vec3(x, y, z) + pos, materialID);
                    }
                }
            }
        }
        mesh->MeshChanged = true;
        BK_INFO("Sphere drawn at: {0} {1} {2}", pos.x, pos.y, pos.z);
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
        std::map<glm::vec3, MATERIAL_ID_TYPE, Vec3Comparator> materials;

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

                MATERIAL_ID_TYPE materialID = 0;

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

    void ModelLoader::SaveMeshToFile(const std::string path, Ref<VoxelMesh> mesh)
    {
        std::ofstream file(path, std::ios::out | std::ios::binary);
        if (!file.is_open())
        {
            BK_CORE_ERROR("Could not open file: {0}", path);
            return;
        }

        glm::uvec3 size = mesh->GetSize();

        file << "D " << size.x << " " << size.y << " " << size.z << std::endl;

        for (int x = 0; x < size.x; x++)
        {
            for (int y = 0; y < size.y; y++)
            {
                for (int z = 0; z < size.z; z++)
                {
                    Voxel voxel = mesh->GetVoxel(glm::vec3(x, y, z));
                    if (voxel == EMPTY_VOXEL)
                        continue;
                    glm::vec3 color = MaterialLibrary::GetMaterial(voxel).Color;
                    file << x << " " << y << " " << z << " " << (int)(color.r * 255) << " " << (int)(color.g * 255) << " " << (int)(color.b * 255) << std::endl;
                }
            }
        }

        file.close();
    }
}
