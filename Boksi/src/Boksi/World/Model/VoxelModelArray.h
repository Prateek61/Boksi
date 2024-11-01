#pragma once

#include "Boksi/World/Model/VoxelModel.h"

namespace Boksi
{
	class VoxelModelArray final: public VoxelModel
	{
	public:
		VoxelModelArray(const glm::uvec3& size);
		VoxelModelArray(const std::vector<Voxel>& voxels, const glm::uvec3& size);
		VoxelModelArray(const VoxelModelArray& other);
		VoxelModelArray(VoxelModelArray&& other) noexcept;
		~VoxelModelArray() override;

		glm::uvec3 GetSize() const override;
		uint32_t GetVoxelCount() const override;
		uint32_t GetModelSizeBytes() const override;
		Voxel GetVoxel(const glm::uvec3& position) const override;
		void SetVoxel(const glm::uvec3& position, const Voxel& voxel) override;
		bool IsModelChanged() const override;
		void ResetModelChanged() override;
	private:
		std::vector<Voxel> m_Voxels;
		glm::uvec3 m_Size;
	};
}