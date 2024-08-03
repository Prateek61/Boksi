#pragma once
#include "bkpch.h"

#include <glm/glm.hpp>
#include "Boksi/World/Voxel.h"

namespace Boksi
{
	// Interface class for VoxelMesh
	class VoxelMesh
	{
	public:
		virtual ~VoxelMesh() = default;

		virtual glm::uvec3 GetSize() const = 0;
		virtual uint32_t GetVoxelCount() const = 0;
		virtual Voxel GetVoxel(const glm::uvec3& position) const = 0;
		virtual void SetVoxel(const glm::uvec3& position, const Voxel& voxel) = 0;
	};
}