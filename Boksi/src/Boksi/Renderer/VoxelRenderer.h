#pragma once
#include "bkpch.h"

#include "Boksi/Renderer/Buffer/Buffer.h"
#include "Boksi/Renderer/Texture.h"
#include "Boksi/Renderer/Camera.h"
#include "Boksi/World/Mesh/VoxelMeshSVO.h"
#include "Boksi/World/Mesh/VoxelMeshArray.h"

namespace Boksi
{
    class VoxelRendererSVO
    {
    public:
        VoxelRendererSVO(const std::string& computeShaderSource);
        ~VoxelRendererSVO() = default;

        void Render(const Camera& camera, const Ref<Texture2D> texture, float voxelSize, Ref<VoxelMeshSVO> mesh, glm::uvec2 resolution = { 1280, 720 }, glm::uvec3 group = { 16, 16, 1 });

        Ref<StorageBuffer> GetStorageBuffer() { return m_VoxelStorageBuffer; }
        Ref<ComputeShader> GetComputeShader() { return m_ComputeShader; }

    private:
        Ref<StorageBuffer> m_VoxelStorageBuffer;
        Ref<ComputeShader> m_ComputeShader;
    };

    class VoxelRendererArray
    {
    public:
        VoxelRendererArray(const std::string& computeShaderSource);
        ~VoxelRendererArray() = default;

        void Render(const Camera& camera, const Ref<Texture2D> texture, Ref<VoxelMeshArray> mesh, float voxelSize, glm::uvec2 resolution = { 1280, 720 }, glm::uvec3 group = { 16, 16, 1 });

        Ref<StorageBuffer> GetStorageBuffer() { return m_VoxelStorageBuffer; }
        Ref<ComputeShader> GetComputeShader() { return m_ComputeShader; }

    private:
        Ref<StorageBuffer> m_VoxelStorageBuffer;
        Ref<ComputeShader> m_ComputeShader;
    };
}