#include "bkpch.h"
#include "VoxelRenderer.h"
#include "Boksi/Renderer/Renderer.h"

namespace Boksi
{
    VoxelRendererSVO::VoxelRendererSVO(const std::string &computeShaderSource)
    {
        // Compute Shader
        const std::string compute_src = ShaderLoader::Load(computeShaderSource);
        m_ComputeShader.reset(ComputeShader::Create(compute_src));

        // Storage Buffer
        m_VoxelStorageBuffer.reset(StorageBuffer::Create());
    }

    void VoxelRendererSVO::Render(const Camera &camera, const Ref<Texture2D> texture, glm::ivec3 dimensions, int maxDepth, float voxelSize, Ref<VoxelMeshSVO> mesh)
    {
        // Bind the compute shader
        m_ComputeShader->Bind();

        // Upload uniforms
        Camera::UploadCameraUniformToShader(m_ComputeShader->UniformUploader, camera);

        m_ComputeShader->UniformUploader->UploadUniformInt("u_MaxDepth", maxDepth);
        m_ComputeShader->UniformUploader->UploadUniformFloat("u_VoxelSize", voxelSize);
        m_ComputeShader->UniformUploader->UploadUniformInt3("u_Dimensions", dimensions);

        m_VoxelStorageBuffer->Bind(1);

        std::vector<GPUOctreeNode> gpuOctreeNodes;

        // Convert Octree to GPUOctree
        FlattenOctree(mesh->GetRoot(), gpuOctreeNodes);

        m_VoxelStorageBuffer->SetData(gpuOctreeNodes.data(), gpuOctreeNodes.size() * sizeof(GPUOctreeNode));
        // Display the size
        BK_INFO("GPUOctree size: {0}", gpuOctreeNodes.size());

        texture->BindWrite(0);

        // Dispatch compute shader
        Boksi::Renderer::DispatchCompute(m_ComputeShader, 1280 / 16, 720 / 16, 1);
    }

}