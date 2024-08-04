#include "bkpch.h"
#include "VoxelMeshModifier.h"
#include "Boksi/World/FastNoiseLite.h"
#include <random> // For generating a random seed

namespace Boksi
{
    void VoxelModifier::DrawCircle(const int radius, const glm::uvec3 &maxSize, const glm::uvec3 &center, const Ref<VoxelMesh> mesh, Voxel voxel)
    {
        for (int i = -radius; i <= radius; i++)
        {
            for (int j = -radius; j <= radius; j++)
            {
                for (int k = -radius; k <= radius; k++)
                {
                    if (i * i + j * j + k * k <= radius * radius)
                    {
                        glm::uvec3 position = center + glm::uvec3(i, j, k);
                        if (glm::all(glm::lessThanEqual(position, maxSize)) && glm::all(glm::greaterThanEqual(glm::uvec3{0, 0, 0}, position)))
                        {
                            mesh->SetVoxel(position, voxel);
                        }
                    }
                }
            }
        }
    }

    void VoxelModifier::Draw(const Ref<VoxelMesh> mesh, EntitiesArray &entitiesArray)
    {
        for (auto &entity : entitiesArray.GetEntities())
        {
            entity->Draw(mesh);
        }
    }

    void VoxelModifier::DrawFloor(const Ref<VoxelMesh> mesh, Voxel voxel)
    {
        FastNoiseLite noise;
        
        glm::ivec3 m_Size = mesh->GetSize();

        float baseHeight = 10.0f;

        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        
        // random seed
        std::random_device rd;
        noise.SetSeed(rd());
        noise.SetFrequency(0.1f); // Adjust the frequency for desired terrain detail

        for (uint32_t x = 0; x < m_Size.x; x++)
        {
            for (uint32_t z = 0; z < m_Size.z; z++)
            {
                // Generate a height value using noise
                float heightVariation = noise.GetNoise((float)x, (float)z) * baseHeight;
                int maxHeight = baseHeight + static_cast<int>(heightVariation);

                for (uint32_t y = 0; y < maxHeight; y++)
                {
                    mesh->SetVoxel(glm::uvec3(x, y, z), voxel);
                }
            }
        }
    }
}