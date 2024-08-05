#pragma once
#include "bkpch.h"
#include "Entity.h"
#include "Boksi/World/Mesh/VoxelMesh.h"

namespace Boksi
{
    class Cube : public Entity
    {
    public:
        Cube() = default;
        Cube(glm::vec3 position, glm::vec3 rotation , size_t size);    

    
        virtual void OnUpdate() override;
        virtual void Draw(Ref<VoxelMesh> mesh) override;
        void Rotate(glm::vec3 rotation);

        glm::vec3 prevPos;

    };
}