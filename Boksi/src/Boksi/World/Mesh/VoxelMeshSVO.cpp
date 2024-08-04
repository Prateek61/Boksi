#include "bkpch.h"
#include "VoxelMeshSVO.h"

namespace Boksi
{
	VoxelMeshSVO::VoxelMeshSVO(const glm::uvec3& size)
		: m_Root(new OctreeNode()), m_Size(size)
	{
		m_MaxDepth = GetMaximumDepth(size);
		m_MaxSize = glm::uvec3(1 << m_MaxDepth);
	}

	VoxelMeshSVO::VoxelMeshSVO(const std::vector<Voxel>& voxels, const glm::uvec3& size)
	{
		m_Root = new OctreeNode();
		m_Size = size;
		m_MaxDepth = GetMaximumDepth(size);
		m_MaxSize = glm::uvec3(1 << m_MaxDepth);

		// Set the voxels
		for (unsigned int x = 0; x < size.x; x++)
		{
			for (unsigned int y = 0; y < size.y; y++)
			{
				for (unsigned int z = 0; z < size.z; z++)
				{
					const Voxel voxel = voxels[x + y * size.x + z * size.x * size.y];
					if (voxel != EMPTY_VOXEL)
					{
						SetVoxel(glm::uvec3(x, y, z), voxel);
					}
				}
			}
		}
	}

	VoxelMeshSVO::~VoxelMeshSVO()
	{
		DeleteNode(m_Root);
	}

	inline Voxel VoxelMeshSVO::GetVoxel(const glm::uvec3& position) const
	{
		return GetVoxel(m_Root, glm::uvec3(0), m_MaxSize, position, m_MaxDepth);
	}

	void VoxelMeshSVO::SetVoxel(const glm::uvec3& position, const Voxel& voxel)
	{
		if (position.x >= m_Size.x || position.y >= m_Size.y || position.z >= m_Size.z)
		{
			BK_CORE_ASSERT(false, "VoxelMeshSVO::SetVoxel: Position is out of bounds")
		}

		bool purge = SetVoxel(m_Root, glm::uvec3(0), m_MaxSize, position, m_MaxDepth, voxel);
	}

	unsigned int VoxelMeshSVO::GetMaximumDepth(const glm::uvec3& size) const
	{
		const unsigned int max_size = std::max(size.x, std::max(size.y, size.z));
		const unsigned depth = static_cast<unsigned int>(std::log2(max_size - 1)) + 1;
		return depth;
	}

	Voxel VoxelMeshSVO::GetVoxel(const OctreeNode* node, const glm::uvec3& min, const glm::uvec3& max, const glm::uvec3& position, unsigned int depth) const
	{
		if (depth == 0)
		{
			BK_CORE_ASSERT(false, "VoxelMeshSVO::GetVoxel: Depth is 0")
		}

		// Compute which child the position is in
		const glm::uvec3 mid = (min + max) / 2u;
		const unsigned int child_index = (position.x >= mid.x ? 1 : 0) + (position.y >= mid.y ? 2 : 0) + (position.z >= mid.z ? 4 : 0);
		// If the child octree doesn't expand, return the voxel
		if (!(node->ChildrenMask & (1 << child_index)))
		{
			return node->ChildrenVoxels[child_index];
		}


		// Compute the new min and max
		const glm::uvec3 new_min = glm::uvec3(
			child_index & 1 ? mid.x : min.x,
			child_index & 2 ? mid.y : min.y,
			child_index & 4 ? mid.z : min.z
		);
		const glm::uvec3 new_max = glm::uvec3(
			child_index & 1 ? max.x : mid.x,
			child_index & 2 ? max.y : mid.y,
			child_index & 4 ? max.z : mid.z
		);

		return GetVoxel(node->Children[child_index], new_min, new_max, position, depth - 1);
	}

	bool VoxelMeshSVO::SetVoxel(OctreeNode* node, const glm::uvec3& min, const glm::uvec3& max, const glm::uvec3& position, unsigned int depth, const Voxel& voxel)
	{
		// Calculate the octant the position is in
		const glm::uvec3 mid = glm::uvec3(min + max) / 2u;
		const unsigned int child_index = (position.x >= mid.x ? 1 : 0) + (position.y >= mid.y ? 2 : 0) + (position.z >= mid.z ? 4 : 0);

		if (depth == 1)
		{
			// If we are at the leaf level, set the voxel
			node->ChildrenVoxels[child_index] = voxel;
			return CheckIfNodePurgeable(node);
		}

		// If we are not at the leaf level, create the child node if it doesn't exist
		if (!(node->ChildrenMask & (1 << child_index)))
		{
			node->ChildrenMask |= 1 << child_index;
			node->Children[child_index] = new OctreeNode();
		}

		// Calculate the new min and max
		const glm::uvec3 new_min = glm::uvec3(
			child_index & 1 ? mid.x : min.x,
			child_index & 2 ? mid.y : min.y,
			child_index & 4 ? mid.z : min.z
		);
		const glm::uvec3 new_max = glm::uvec3(
			child_index & 1 ? max.x : mid.x,
			child_index & 2 ? max.y : mid.y,
			child_index & 4 ? max.z : mid.z
		);

		// Recurse
		bool purge = SetVoxel(node->Children[child_index], new_min, new_max, position, depth - 1, voxel);
		if (!purge)
		{
			return false;
		}

		// Purge the child node
		// Set the bit mask for the child to 1
		node->ChildrenMask &= ~(1 << child_index);
		// Set the child voxel
		node->ChildrenVoxels[child_index] = node->Children[child_index]->ChildrenVoxels[0];
		// Delete the child node
		delete node->Children[child_index];
		// Set the child node to nullptr
		node->Children[child_index] = nullptr;

		// Check if the parent node can be purged
		return CheckIfNodePurgeable(node);
	}

	bool VoxelMeshSVO::CheckIfNodePurgeable(const OctreeNode* node) const
	{
		if (node->ChildrenMask != 0)
		{
			return false;
		}

		// Check if all the children have the same voxel
		const auto& voxel = node->ChildrenVoxels[0];
		for (auto itr = std::begin(node->ChildrenVoxels) + 1; itr != std::end(node->ChildrenVoxels); ++itr)
		{
			if (*itr != voxel)
			{
				return false;
			}
		}

		return true;
	}

	void VoxelMeshSVO::DeleteNode(OctreeNode* node)
	{
		if (node == nullptr)
		{
			return;
		}

		// Loop over all the children and delete them
		for (auto child : node->Children)
		{
			DeleteNode(child);
		}

		// Delete the node
		delete node;
	}
	
	void FlattenOctree(OctreeNode* node, std::vector<GPUOctreeNode>& flatGPUOctree)
	{
		if (!node)
		{
			return;
		}

		int index = flatGPUOctree.size();
		// Create the GPUOctreeNode
		GPUOctreeNode gpu_node(*node);
		flatGPUOctree.push_back(gpu_node);

		for (int i = 0; i < 8; ++i)
		{
			if (node->ChildrenMask & (1 << i))
			{
				flatGPUOctree[index].Children[i] = static_cast<uint8_t>(flatGPUOctree.size());
				FlattenOctree(node->Children[i], flatGPUOctree);
			}
		}
	}
}