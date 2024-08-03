#pragma once
#include "bkpch.h"

#include "Boksi/World/Mesh/VoxelMesh.h"

namespace Boksi
{
	struct OctreeNode
	{
		uint8_t ChildrenMask;
		OctreeNode* Children[8];
		Voxel ChildrenVoxels[8];

		OctreeNode()
			: ChildrenMask(0), Children{ nullptr }, ChildrenVoxels{ EMPTY_VOXEL }
		{}
	};

	struct GPUOctreeNode
	{
		uint8_t ChildrenMask;
		int Children[8];
		Voxel ChildrenVoxels[8];

		GPUOctreeNode(const OctreeNode& octreeNode)
			: ChildrenMask(octreeNode.ChildrenMask), Children{ -1, -1, -1, -1, -1, -1, -1, -1 }, ChildrenVoxels{ EMPTY_VOXEL }
		{
			for (int i = 0; i < 8; i++)
			{
				if (octreeNode.Children[i] != nullptr)
				{
					Children[i] = 0;
					ChildrenVoxels[i] = octreeNode.ChildrenVoxels[i];
				}
			}
		}
	};

	class VoxelMeshSVO : public VoxelMesh
	{
	public:
		VoxelMeshSVO(const glm::uvec3& size);
		VoxelMeshSVO(const std::vector<Voxel>& voxels, const glm::uvec3& size);
		~VoxelMeshSVO() override;

	public:
		inline glm::uvec3 GetSize() const override;
		inline uint32_t GetVoxelCount() const override;

		Voxel GetVoxel(const glm::uvec3& position) const override;
		void SetVoxel(const glm::uvec3& position, const Voxel& voxel) override;

	private:
		unsigned int GetMaximumDepth(const glm::uvec3& size) const;

		Voxel GetVoxel(const OctreeNode* node, const glm::uvec3& min, const glm::uvec3& max, const glm::uvec3& position, unsigned int depth) const;
		bool SetVoxel(OctreeNode* node, const glm::uvec3& min, const glm::uvec3& max, const glm::uvec3& position, unsigned int depth, const Voxel& voxel);
		bool CheckIfNodePurgeable(const OctreeNode* node) const;

		void DeleteNode(OctreeNode* node);

	private:
		OctreeNode* m_Root;
		unsigned int m_MaxDepth;
		glm::uvec3 m_Size;
		glm::uvec3 m_MaxSize;
	};

	void FlattenOctree(OctreeNode* node, std::vector<GPUOctreeNode>& flatGPUOctree);
}

// Inline definitions
namespace Boksi
{
	inline glm::uvec3 VoxelMeshSVO::GetSize() const
	{
		return m_Size;
	}

	inline uint32_t VoxelMeshSVO::GetVoxelCount() const
	{
		return m_Size.x * m_Size.y * m_Size.z;
	}
}