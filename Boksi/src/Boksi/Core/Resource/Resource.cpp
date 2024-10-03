#include "bkpch.h"
#include "Resource.h"

#include "Boksi/Core/Resource/FileIO.h"

namespace Boksi
{
    BinaryResource::~BinaryResource()
    {
        BK_PROFILE_FUNCTION();

		if (m_DataBuffer)
		{
			BK_CORE_WARN("BinaryResource::~BinaryResource: Resource {0} is being destroyed while still loaded", GetResourcePath().string());
			BinaryResource::UnloadResource();
		}
    }

    void BinaryResource::LoadResource()
    {
        BK_PROFILE_FUNCTION();

        if (m_DataBuffer)
        {
            BK_CORE_TRACE("BinaryResource::LoadResource: Reloading resource {0}", GetResourcePath().string());
            m_DataBuffer.Release();
		}
		else
		{
			BK_CORE_TRACE("BinaryResource::LoadResource: Loading resource {0}", GetResourcePath().string());
		}

		m_DataBuffer = FileIO::ReadFileBinary(GetResourcePath());
    }

    void BinaryResource::UnloadResource()
    {
        BK_PROFILE_FUNCTION();

		if (m_DataBuffer)
		{
			BK_CORE_TRACE("BinaryResource::UnloadResource: Unloading resource {0}", GetResourcePath().string());
			m_DataBuffer.Release();
		}
		else
		{
			BK_CORE_WARN("BinaryResource::UnloadResource: Resource {0} is already unloaded", GetResourcePath().string());
		}
    }

    void BinaryResource::StoreToFile()
    {
        BK_PROFILE_FUNCTION();

        if ( !m_DataBuffer )
        {
			BK_CORE_WARN("BinaryResource::StoreToFile: No data to store for resource {0}", GetResourcePath().string());
			return;
        }

		FileIO::WriteFileBinary(GetResourcePath(), m_DataBuffer);
    }

    Buffer BinaryResource::StealBuffer()
    {
        BK_PROFILE_FUNCTION();

        Buffer buffer = m_DataBuffer;
        m_DataBuffer = Buffer();
        return buffer;
    }

    Buffer BinaryResource::CopyBuffer() const
    {
        BK_PROFILE_FUNCTION();

        return Buffer::Copy(m_DataBuffer);
    }

    void BinaryResource::SetDataBuffer(Buffer buffer)
    {
        BK_PROFILE_FUNCTION();

        if ( m_DataBuffer )
        {
            UnloadResource();
        }

        m_DataBuffer = buffer;
    }
}