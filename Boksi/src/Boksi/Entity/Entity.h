#pragma once
#include "bkpch.h"
#include "glm/glm.hpp"
#include "Boksi/World/Mesh/VoxelMesh.h"

namespace Boksi
{
    class Entity
    {

    public:
        Entity() = default;
        Entity(const Entity &other) = default;
        Entity(Entity &&other) = default;
        Entity &operator=(const Entity &other) = default;
        Entity &operator=(Entity &&other) = default;
        virtual ~Entity() = default;
        virtual void OnUpdate() = 0;
        virtual void Draw(Ref<VoxelMesh> mesh) = 0;
        virtual void CreateModel(std::vector<std::string> model) = 0;

        void SetModel(std::vector<std::string> model) { m_Model = model; }
        std::vector<std::string> GetModel() { return m_Model; }
        glm::vec3 GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec3 &position) { m_Position = position; }
        glm::vec3 GetRotation() const { return m_Rotation; }
        void SetRotation(const glm::vec3 &rotation) { m_Rotation = rotation; }
        void SetDimension(glm::vec3 dimension) { m_Dimension = dimension; }
        glm::vec3 GetDimension() { return m_Dimension; }

    private:
        glm::vec3 m_Position;
        glm::vec3 m_Rotation;
        std::vector<std::string> m_Model;
        glm::vec3 m_Dimension;
    };
}
