#include "bkpch.h"
#include "Cube.h"

namespace Boksi
{
    Cube::Cube(glm::vec3 position, glm::vec3 rotation, size_t size)
    {
        SetPosition(position);
        SetRotation(rotation);
    }

    void Cube::OnUpdate()
    {
        // Do something
    }

    void Cube::Draw(Ref<VoxelMesh> mesh)
    {
        // Draw the cube
        for (size_t i = 0; i < 6; i++)
        {
            for (size_t j = 0; j < 6; j++)
            {
                for (size_t k = 0; k < 6; k++)
                {
                    mesh->SetVoxel(glm::uvec3(i, j, k), uint8_t(1));
                }
            }
        }
    }
}