#include "bkpch.h"
#include "VoxelMeshModifier.h"

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
}