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
		uint8_t ValidMask;
		uint8_t Padding[2]; // Align next member to 4 byte
		Voxel ChildrenVoxels[8];
		int Children[8];

		GPUOctreeNode() = default;
		GPUOctreeNode(const OctreeNode& octreeNode)
			: ChildrenMask(octreeNode.ChildrenMask), ChildrenVoxels{ EMPTY_VOXEL }, Children{ -1, -1, -1, -1, -1, -1, -1, -1 }
		{
			for (int i = 0; i < 8; i++)
			{
				ChildrenVoxels[i] = octreeNode.ChildrenVoxels[i];

				if (!(!(octreeNode.ChildrenMask & (1 << i)) && ChildrenVoxels[i] == EMPTY_VOXEL))
				{
					ValidMask |= (1 << i);
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
		OctreeNode *GetRoot() { return m_Root; }
		unsigned int GetMaximumDepth() const { return m_MaxDepth; }

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

	inline void FlattenOctree(const OctreeNode* node, std::vector<GPUOctreeNode>& flatGPUOctree)
	{
		if (!node)
		{
			BK_CORE_ASSERT(false, "Should not be null")
				return;
		}

		int index = flatGPUOctree.size();
		// Create the GPUOctreeNode
		GPUOctreeNode gpu_node(*node);
		flatGPUOctree.push_back(gpu_node);

		// Process children
		for (int i = 0; i < 8; i++)
		{
			if (node->ChildrenMask & (1 << i))
			{
				// Record the index of the child node
				flatGPUOctree[index].Children[i] = static_cast<int>(flatGPUOctree.size());
				// Recurse into the child node
				FlattenOctree(node->Children[i], flatGPUOctree);
			}
			else
			{
				// No child, set the index to -1
				flatGPUOctree[index].Children[i] = -1;
			}
		}
	}
}