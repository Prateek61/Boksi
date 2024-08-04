#pragma once
#include "bkpch.h"

#include "Boksi/Renderer/Buffer/Buffer.h"
#include "Boksi/Renderer/Texture.h"
#include "Boksi/Renderer/Camera.h"
#include "Boksi/World/Mesh/VoxelMeshArray.h"

namespace Boksi
{
    class VoxelRendererSVO
    {
    public:
        VoxelRendererSVO(const std::string& computeShaderSource);
        ~VoxelRendererSVO() = default;

        void Render(const Camera& camera, const Ref<Texture2D> texture, glm::ivec3 dimensions, int maxDepth, float voxelSize , Ref<VoxelMeshArray> mesh);

    private:
        Ref<StorageBuffer> m_VoxelStorageBuffer;
        Ref<ComputeShader> m_ComputeShader;
    };
}