#pragma once
#include "bkpch.h"

#include "Boksi/Renderer/Buffer/StorageBuffer.h"

namespace Boksi
{
    class OpenGLStorageBuffer : public StorageBuffer
    {
    public:
        OpenGLStorageBuffer();
    	~OpenGLStorageBuffer() override;

        void Bind(uint32_t slot) const override;
        void Unbind() const override;

        void SetData(void* data, uint32_t size) override;

    private:
        uint32_t m_RendererID;
    };
}