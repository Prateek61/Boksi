#include "bkpch.h"

#include "World.h"
#include "FastNoiseLite.h"

namespace Boksi
{
    void World::DrawCircle(const int x, const int y, const int z, const int radius, const MATERIAL_ID_TYPE materialID)
    {
        for (int i = -radius; i <= radius; i++)
        {
            for (int j = -radius; j <= radius; j++)
            {
                for (int k = -radius; k <= radius; k++)
                {
                    if (i * i + j * j + k * k <= radius * radius)
                    {
                        if (x + i >= 0 && x + i < m_Size.x && y + j >= 0 && y + j < m_Size.y && z + k >= 0 && z + k < m_Size.z)
                        {
                            SetVoxel(glm::uvec3(x + i, y + j, z + k), materialID);
                        }
                    }
                }
            }
        }
    }

    void World::AddBoundary(const MATERIAL_ID_TYPE materialID)
    {
        // only add boundary lines at edges
        for (uint32_t x = 0; x < m_Size.x; x++)
        {
            for (uint32_t y = 0; y < m_Size.y; y++)
            {
                for (uint32_t z = 0; z < m_Size.z; z++)
                {
                    if (x == 0 || x == m_Size.x - 1 || y == 0 || y == m_Size.y - 1 || z == 0 || z == m_Size.z - 1)
                    {
                        SetVoxel(glm::uvec3(x, y, z), materialID);
                    }
                }
            }
        }
    }

    void World::ClearScreen(const MATERIAL_ID_TYPE materialID)
    {
        for (uint32_t x = 0; x < m_Size.x; x++)
        {
            for (uint32_t y = 0; y < m_Size.y; y++)
            {
                for (uint32_t z = 0; z < m_Size.z; z++)
                {
                    SetVoxel(glm::uvec3(x, y, z), materialID);
                }
            }
        }
    }


    void World::AddWorldFloor(const int baseHeight, const MATERIAL_ID_TYPE materialID)
    {
        FastNoiseLite noise;
        
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise.SetFrequency(0.01f); // Adjust the frequency for desired terrain detail

        for (uint32_t x = 0; x < m_Size.x; x++)
        {
            for (uint32_t z = 0; z < m_Size.z; z++)
            {
                // Generate a height value using noise
                float heightVariation = noise.GetNoise((float)x, (float)z) * baseHeight;
                int maxHeight = baseHeight + static_cast<int>(heightVariation);

                for (uint32_t y = 0; y < maxHeight; y++)
                {
                    SetVoxel(glm::uvec3(x, y, z), materialID);
                }
            }
        }
    }

}