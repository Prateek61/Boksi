#pragma once
#include "bkpch.h"

namespace Boksi
{
    class StorageBuffer
    {
    public:
        virtual ~StorageBuffer() = default;

        virtual void Bind(uint32_t slot) const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(void* data, uint32_t size) = 0;

        static StorageBuffer* Create();
    };
}
