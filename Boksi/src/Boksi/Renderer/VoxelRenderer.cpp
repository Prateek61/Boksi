#include "bkpch.h"
#include "VoxelRenderer.h"
#include "Boksi/Renderer/Renderer.h"
#include "Boksi/World/Mesh/VoxelMeshSVO.h"

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

    void VoxelRendererSVO::Render(const Camera &camera, const Ref<Texture2D> texture, float voxelSize, Ref<VoxelMeshSVO> mesh, glm::uvec2 resolution, glm::uvec3 group)
    {
        // Bind the compute shader
        m_ComputeShader->Bind();

        // Upload uniforms
        Camera::UploadCameraUniformToShader(m_ComputeShader->UniformUploader, camera);

        m_ComputeShader->UniformUploader->UploadUniformInt("u_MaxDepth", mesh->GetMaximumDepth());
        m_ComputeShader->UniformUploader->UploadUniformFloat("u_VoxelSize", voxelSize);
        m_ComputeShader->UniformUploader->UploadUniformInt3("u_Dimensions", mesh->GetSize());
        m_ComputeShader->UniformUploader->UploadUniformInt("u_MaxDepth", mesh->GetMaximumDepth());

        m_VoxelStorageBuffer->Bind(1);

        if (mesh->MeshChanged)
        {
	        mesh->MeshChanged = false;
			// Convert VoxelMesh to GPUOctree
			std::vector<GPUOctreeNode> gpuOctreeNodes;
			FlattenOctree(mesh->GetRoot(), gpuOctreeNodes);

			m_VoxelStorageBuffer->SetData(gpuOctreeNodes.data(), gpuOctreeNodes.size() * sizeof(GPUOctreeNode));

            // Display Flatten size
        	// BK_CORE_TRACE("Flatten size: {0}", gpuOctreeNodes.size());
        }

        texture->BindWrite(0);

        // Dispatch compute shader
        Boksi::Renderer::DispatchCompute(m_ComputeShader, resolution.x / group.x, resolution.y / group.y, group.z);
    }

    VoxelRendererArray::VoxelRendererArray(const std::string& computeShaderSource)
    {
        // Compute Shader
        const std::string compute_src = ShaderLoader::Load(computeShaderSource);
        m_ComputeShader.reset(ComputeShader::Create(compute_src));

        // Storage Buffer
        m_VoxelStorageBuffer.reset(StorageBuffer::Create());
    }

    void VoxelRendererArray::Render(const Camera& camera, const Ref<Texture2D> texture, Ref<VoxelMeshArray> mesh, float voxelSize, glm::uvec2 resolution, glm::uvec3 group)
    {
        // Bind the compute shader
        m_ComputeShader->Bind();

        // Upload uniforms
        Camera::UploadCameraUniformToShader(m_ComputeShader->UniformUploader, camera);
        m_ComputeShader->UniformUploader->UploadUniformFloat("u_VoxelSize", voxelSize);
        m_ComputeShader->UniformUploader->UploadUniformInt3("u_Dimensions", mesh->GetSize());

        // Storage Buffer
        m_VoxelStorageBuffer->Bind(0);

        if (true)
		{
			mesh->MeshChanged = false;
			m_VoxelStorageBuffer->SetData(mesh->GetVoxelArray().data(), mesh->GetVoxelCount() * sizeof(Voxel));
		}

        texture->BindWrite(0);

        

        // Dispatch compute shader
        Boksi::Renderer::DispatchCompute(m_ComputeShader, resolution.x / group.x, resolution.y / group.y, group.z);
    }

}
