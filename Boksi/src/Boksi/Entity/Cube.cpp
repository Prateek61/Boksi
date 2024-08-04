#include "bkpch.h"
#include "Cube.h"

namespace Boksi
{
    Cube::Cube(glm::vec3 position, glm::vec3 rotation, size_t size)
    {
        SetPosition(position);
        prevPos = position;
        SetRotation(rotation);
    }

    void Cube::OnUpdate()
    {
        SetPosition(GetPosition() + glm::vec3(1.0f, 1.0f, 1.0f));
        if (GetPosition().x > 40.0f)
        {
            SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        }
    }

    void Cube::Draw(Ref<VoxelMesh> mesh)
    {
        // Check if the position has changed
        if (prevPos != GetPosition())
        {
            // Define the size of the cube
            const glm::ivec3 cubeSize(6, 6, 6);

            // Remove the old cube
            for (int i = 0; i < cubeSize.x; i++)
            {
                for (int j = 0; j < cubeSize.y; j++)
                {
                    for (int k = 0; k < cubeSize.z; k++)
                    {
                        glm::uvec3 oldPos = glm::uvec3(prevPos) + glm::uvec3(i, j, k);
                        mesh->SetVoxel(oldPos, EMPTY_VOXEL);
                    }
                }
            }

            // Update the previous position
            prevPos = GetPosition();

            // Calculate the new position as an integer vector
            glm::ivec3 newPos = glm::ivec3(GetPosition());

            // Draw the new cube
            for (int i = 0; i < cubeSize.x; i++)
            {
                for (int j = 0; j < cubeSize.y; j++)
                {
                    for (int k = 0; k < cubeSize.z; k++)
                    {
                        glm::uvec3 currentPos = glm::uvec3(newPos) + glm::uvec3(i, j, k);
                        mesh->SetVoxel(currentPos, 1); // Assuming '1' is the filled voxel value
                    }
                }
            }
        }
    }

}