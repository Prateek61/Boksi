// Class representing the voxel world in a sparse voxel octree structure

#pragma once
#include "bkpch.h"

#include "Boksi/World/Voxel.h"
#include "Boksi/World/Material.h"
#include <glm/glm.hpp>

namespace Boksi
{
    class World
    {
    public:
        World(const glm::uvec3& size)
            : m_Size(size)
        {
            m_Voxels.resize(size.x * size.y * size.z);
        }

        // Getters
        const glm::uvec3& GetSize() const { return m_Size; }
        const Voxel& GetVoxel(const glm::uvec3& position) const { return m_Voxels[position.x + position.y * m_Size.x + position.z * m_Size.x * m_Size.y]; }
        const std::vector<Voxel>& GetVoxels() const { return m_Voxels; }

        // Setters
        void SetVoxel(const glm::uvec3& position, const uint8_t materialID) { m_Voxels[position.x + position.y * m_Size.x + position.z * m_Size.x * m_Size.y].SetMaterialID(materialID); }
        void SetVoxel(const glm::uvec3& position, const Voxel& voxel) { m_Voxels[position.x + position.y * m_Size.x + position.z * m_Size.x * m_Size.y] = voxel; }
        void UnfillVoxel(const glm::uvec3& position) { m_Voxels[position.x + position.y * m_Size.x + position.z * m_Size.x * m_Size.y].Unfill(); }
    private:
        std::vector<Voxel> m_Voxels;
        glm::uvec3 m_Size;
    };
}