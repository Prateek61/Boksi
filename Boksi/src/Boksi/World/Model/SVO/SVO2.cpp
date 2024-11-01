#include "bkpch.h"

#include "SVO2.h"

namespace Boksi
{
	inline uint32_t SVO2MaxDepth(const glm::uvec3& size)
	{
		const uint32_t max_size = std::max(size.x, std::max(size.y, size.z));
		const uint32_t max_depth = static_cast<uint32_t>(std::log2(max_size - 1)) + 1;
		return max_depth;
	}

	inline int GetChildIdx(const glm::uvec3& min, const glm::uvec3& max, const glm::uvec3& position)
	{
		int idx = 0;
		if (position.x >= min.x + (max.x - min.x) / 2) idx |= 1;
		if (position.y >= min.y + (max.y - min.y) / 2) idx |= 2;
		if (position.z >= min.z + (max.z - min.z) / 2) idx |= 4;
		return idx;
	}

	SVO2::SVO2(const glm::uvec3& size)
		: m_Size(size), m_Nodes{ 1 }, m_IsDirty(false), m_IsChanged(false)
	{
		m_MaxDepth = SVO2MaxDepth(size);
		m_TotalSize = glm::uvec3(1 << m_MaxDepth);

		// The index 0 is reserved for empty node
		BK_CORE_ASSERT(m_Nodes.size() == 1, "Nooooooo, the size needs to be 1")

		// Create the root node
		m_Nodes.emplace_back();
	}

	Voxel SVO2::GetVoxel(const glm::uvec3& position) const
	{
		return InternalGetVoxel(position);
	}

	Voxel SVO2::InternalGetVoxel(const glm::uvec3& position) const
	{
		// Check if the position is out of bounds
		if (position.x >= m_Size.x || position.y >= m_Size.y || position.z >= m_Size.z)
		{
			BK_CORE_ASSERT(false, "SVO2::GetVoxel: Position is out of bounds")
			return EMPTY_VOXEL;
		}

		// Root node
		uint32_t index = 1;
		uint32_t depth = 0;

		glm::uvec3 min = glm::uvec3(0);
		glm::uvec3 max = m_TotalSize;

		// Traverse the tree
		while(depth < m_MaxDepth)
		{
			const Svo2Node& node = m_Nodes[index];

			// Get the child index
			glm::uvec3 mid = (min + max) / 2u;
			int child_idx = 0;
			if (position.x >= mid.x) child_idx |= 1;
			if (position.y >= mid.y) child_idx |= 2;
			if (position.z >= mid.z) child_idx |= 4;

			if (node.ChildrenMask & (1 << child_idx))
			{
				// The child tree exists
				index = node.ChildrenIdx[child_idx];
				depth++;

				min.x = child_idx & 1 ? mid.x : min.x;
				max.x = child_idx & 1 ? max.x : mid.x;
				min.y = child_idx & 2 ? mid.y : min.y;
				max.y = child_idx & 2 ? max.y : mid.y;
				min.z = child_idx & 4 ? mid.z : min.z;
				max.z = child_idx & 4 ? max.z : mid.z;
			}
			else
			{
				// The child tree does not exist
				return node.ChildrenVoxels[child_idx];
			}
		}

		BK_CORE_ASSERT(false, "SVO2::GetVoxel: Depth is max")
		return EMPTY_VOXEL;
	}

}