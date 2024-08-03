#pragma once
#include "bkpch.h"

#include "Boksi/World/Mesh/VoxelMesh.h"

namespace Boksi
{
	class VoxelMeshArray final : public VoxelMesh
	{
	public:
		VoxelMeshArray(const glm::uvec3& size);
		~VoxelMeshArray() override = default;

	public:
		// Getters
		inline glm::uvec3 GetSize() const override;
		inline uint32_t GetVoxelCount() const override;
		inline Voxel GetVoxel(const glm::uvec3& position) const override;
		inline void SetVoxel(const glm::uvec3& position, const Voxel& voxel) override;

		inline const std::vector<Voxel>& GetVoxelArray() const;
		inline std::vector<Voxel>& GetVoxelArray();

	private:
		std::vector<Voxel>m_Voxels;
		glm::uvec3 m_Size;
	};
}

// Inline definitions
namespace Boksi
{
	inline glm::uvec3 VoxelMeshArray::GetSize() const
	{
		return m_Size;
	}

	inline uint32_t VoxelMeshArray::GetVoxelCount() const
	{
		return  static_cast<uint32_t>(m_Voxels.size());
	}

	inline Voxel VoxelMeshArray::GetVoxel(const glm::uvec3& position) const
	{
		return m_Voxels[position.x + position.y * m_Size.x + position.z * m_Size.x * m_Size.y];
	}

	inline void VoxelMeshArray::SetVoxel(const glm::uvec3& position, const Voxel& voxel)
	{
		m_Voxels[position.x + position.y * m_Size.x + position.z * m_Size.x * m_Size.y] = voxel;
	}

	inline const std::vector<Voxel>& VoxelMeshArray::GetVoxelArray() const
	{
		return m_Voxels;
	}

	inline std::vector<Voxel>& VoxelMeshArray::GetVoxelArray()
	{
		return m_Voxels;
	}
}