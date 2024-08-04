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

        World(const glm::uvec3& size, const Voxel fillVoxel)
	        : m_Size(size)
        {
	        m_Voxels.resize(size.x * size.y * size.z, fillVoxel);
        }

        // Getters
        const glm::uvec3& GetSize() const { return m_Size; }
        const uint32_t GetVoxelCount() const { return m_Size.x * m_Size.y * m_Size.z; }
        const Voxel& GetVoxel(const glm::uvec3& position) const { return m_Voxels[position.x + position.y * m_Size.x + position.z * m_Size.x * m_Size.y]; }
        const std::vector<Voxel>& GetVoxels() const { return m_Voxels; }
        std::vector<Voxel>& GetVoxels() { return m_Voxels; }
        const void* GetVoxelsData() const { return m_Voxels.data(); }
        void* GetVoxelsData() { return m_Voxels.data(); }
        

        // Setters
        // void SetVoxel(const glm::uvec3& position, const uint8_t materialID) { m_Voxels[position.x + position.y * m_Size.x + position.z * m_Size.x * m_Size.y] = materialID; }
        void SetVoxel(const glm::uvec3& position, const Voxel& voxel) { m_Voxels[position.x + position.y * m_Size.x + position.z * m_Size.x * m_Size.y] = voxel; }
        void UnfillVoxel(const glm::uvec3& position) { m_Voxels[position.x + position.y * m_Size.x + position.z * m_Size.x * m_Size.y] = 0; }

        void DrawCircle(const int x , const int y , const int z , const int radius , const uint8_t materialID);

        void AddBoundary(const uint8_t materialID);

        void ClearScreen(const uint8_t materialID);

        void AddWorldFloor(const int height , const uint8_t materialID);
          

        // Utility functions
        void Randomize(const float density, const std::vector<uint8_t>& materialIDs)
        {
	        for (uint32_t x = 0; x < m_Size.x; x++)
	        {
		        for (uint32_t y = 0; y < m_Size.y; y++)
		        {
			        for (uint32_t z = 0; z < m_Size.z; z++)
			        {
				        if (rand() / (float)RAND_MAX <= density)
				        {
					        SetVoxel(glm::uvec3(x, y, z), materialIDs[rand() % materialIDs.size()]);
				        }
			        }
		        }
	        }
        }

    private:
        std::vector<Voxel> m_Voxels;
        glm::uvec3 m_Size;
    };
}