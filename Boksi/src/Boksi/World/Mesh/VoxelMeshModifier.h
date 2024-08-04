#pragma once
#include "bkpch.h"

#include <glm/glm.hpp>
#include "Boksi/World/Mesh/VoxelMesh.h"
#include "Boksi/World/Voxel.h"

namespace Boksi
{
    class VoxelModifier
    {
    public:
        static void DrawCircle(const int radius, const glm::uvec3& maxSize, const glm::uvec3& center, const Ref<VoxelMesh> mesh, Voxel voxel);
        
    };
}