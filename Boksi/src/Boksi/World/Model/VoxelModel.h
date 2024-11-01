#pragma once

#include "Boksi/World/Voxel.h"

namespace Boksi
{
	// Interface class for VoxelModel
	class VoxelModel
	{
	public:
		virtual ~VoxelModel() = default;

		virtual glm::uvec3 GetSize() const = 0;
		virtual uint32_t GetVoxelCount() const = 0;
		virtual uint32_t GetModelSizeBytes() const = 0;
		virtual Voxel GetVoxel(const glm::uvec3& position) const = 0;
		virtual void SetVoxel(const glm::uvec3& position, const Voxel& voxel) = 0;
		 
		virtual bool IsModelChanged() const = 0;
		virtual void ResetModelChanged() = 0;
	};
}