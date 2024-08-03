#include "bkpch.h"
#include "VoxelRenderer.h"

namespace Boksi
{
    VoxelRendererSVO::VoxelRendererSVO(const std::string& computeShaderSource)
    {
        // Compute Shader
        const std::string compute_src = ShaderLoader::Load(computeShaderSource);
        m_ComputeShader.reset(ComputeShader::Create(compute_src));

        // Storage Buffer
        m_VoxelStorageBuffer.reset(StorageBuffer::Create());
    }
}