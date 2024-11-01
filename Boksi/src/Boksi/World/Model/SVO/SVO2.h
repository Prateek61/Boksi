#pragma once

#include "Boksi/World/Material.h"
#include "Boksi/World/Voxel.h"
#include "Boksi/World/Model/VoxelModel.h"

#include <vector>

namespace Boksi
{
	struct Svo2Node
	{
		uint8_t ChildrenMask;
		uint8_t ValidMask;
		uint8_t Padding[2]; // Align next member to 4 byte
		uint32_t ChildrenIdx[8];
		Voxel ChildrenVoxels[8];

		Svo2Node()
			: ChildrenMask(0), ValidMask(0), Padding{ 0 }, ChildrenIdx{ 0 }, ChildrenVoxels{ 0ll }
		{}
	};

	// Octree Model with 2 divisions per axis
	class SVO2 final: public VoxelModel
	{
	public:
		SVO2(const glm::uvec3& size);
		SVO2(const std::vector<Voxel>& voxels, const glm::uvec3& size);
		~SVO2() = default;

	public:
		glm::uvec3 GetSize() const override;
		uint32_t GetVoxelCount() const override;
		uint32_t GetModelSizeBytes() const override;
		Voxel GetVoxel(const glm::uvec3& position) const override;
		void SetVoxel(const glm::uvec3& position, const Voxel& voxel) override;

		bool IsModelChanged() const override;
		void ResetModelChanged() override;

	public:
		bool IsDirty() const;
		uint32_t GetMaxDepth() const;
		void Optimize();

		void* GetGPUData() const;

	private:
		Voxel InternalGetVoxel(const glm::uvec3& position) const;

	private:
		glm::uvec3 m_Size;
		glm::uvec3 m_TotalSize;
		uint32_t m_MaxDepth;
		std::vector<Svo2Node> m_Nodes;
		bool m_IsDirty = true;
		bool m_IsChanged = true;
	};
}

// Definitions
namespace Boksi
{
	inline glm::uvec3 SVO2::GetSize() const
	{
		return m_Size;
	}

	inline uint32_t SVO2::GetVoxelCount() const
	{
		return m_Size.x * m_Size.y * m_Size.z;
	}

	inline uint32_t SVO2::GetModelSizeBytes() const
	{
		return static_cast<uint32_t>(m_Nodes.size() * sizeof(Svo2Node));
	}

	inline bool SVO2::IsModelChanged() const
	{
		return m_IsChanged;
	}

	inline void SVO2::ResetModelChanged()
	{
		m_IsChanged = false;
	}

	inline bool SVO2::IsDirty() const
	{
		return m_IsDirty;
	}

	inline uint32_t SVO2::GetMaxDepth() const
	{
		return m_MaxDepth;
	}

    inline void* SVO2::GetGPUData() const
    {
        return const_cast<void*>(reinterpret_cast<const void*>(m_Nodes.data()));
    }
}