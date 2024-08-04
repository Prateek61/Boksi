#include "bkpch.h"
#include "VoxelMeshArray.h"

namespace Boksi
{
	VoxelMeshArray::VoxelMeshArray(const glm::uvec3& size)
		: m_Size(size)
	{
		// 	 ⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣄⡀
		// ⠀⣠⣿⣿⣿⠟⠉⠁⠈⠉⠉⠉⠙⠛⢻⣿⣿⣿⣿⣿⣿⠛⠛⠛⠛⠛⠋⠑
		// ⢸⣿⣿⣿⣿⣷⠶⠾⠷⠶⠶⠶⠒⠒⠛⢿⣿⣿⣿⣿⣿⠦⢠⠖⠲⠶⢶⣿⣶⡄
		// ⣼⣿⣿⣿⠋⠀⠀⠀⠀⠀⠀⢰⠖⠀⠀⠀ ⣿⣿⣿⡟⠙⠸⡏⠀⠀⠰⠀⣈⣿⠇
		// ⣿⣿⣿⣿⣿⣿⣿⣶⣶⣴⣶⡿⣀⣤⣤⣾⣿⣿⣿⣿⣤⣤⣿⣶⣶⣿⣿⠿⠋⠀
		// ⣿⣿⣿⣿⣿⣿⡿⠿⠿⣿⣿⣿⣿⣿⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇
		// ⣿⣿⣿⣿⣿⡏⣴⣶⣶⣬⠻⣿⣿⣧⣾⣿⣿⣿⣿⣿⣿⣿⠿⠿⢿⣿⣿⣆
		// ⠻⣿⣿⣿⣿⠃⠘⢿⣿⣿⣷⠘⠿⣿⣯⣽⣾⣿⠿⠋⣉⣤⣴⣶⡆⢹⣿⣿⡆⠀
		// ⣀⠽⣿⣿⣿⣷⡆⢸⣿⣿⣿⡇⢀⣀⠀⠀⠈⣀⣴⣾⣿⣿⠿⠛⠁⣈⣉⠛
		// ⣻⣿⣿⣿⣿⡿⠁⣼⣿⣿⣿⠁⣈⣀⣤⣾⣿⣿⠿⠟⠉⣀⣤⣶⣿⠿⠛
		// ⣿⣿⣿⣿⠏⠁⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣶⣶⡄⢹⡿⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
		m_Voxels.resize(static_cast<std::vector<Boksi::Voxel, std::allocator<Boksi::Voxel>>::size_type>(size.x) * size.y * size.z, EMPTY_VOXEL);
	}
}