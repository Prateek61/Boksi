#pragma once
#include "bkpch.h"
#include "Entity.h"
#include "Boksi/World/Mesh/VoxelMesh.h"

namespace Boksi
{
    class Object : public Entity
    {
    public:
        Object() = default;
        Object(glm::vec3 position, glm::vec3 rotation, size_t size, std::string name);

        virtual void OnUpdate() override;
        virtual void Draw(Ref<VoxelMesh> mesh) override;
        virtual void CreateModel(std::vector<std::string> model) override;

        void FillMesh(Ref<VoxelMesh> mesh, glm::vec3 position);
        void ClearMesh(Ref<VoxelMesh> mesh, glm::vec3 position);

        glm::vec3 prevPos = glm::vec3(-100, -100, -100);

    private:
        std::string m_Name;
        float elapsedTime = 0.0f; // Add this member variable
        float timeStep = 0.016f;  // Assume 60 updates per second
    };
}