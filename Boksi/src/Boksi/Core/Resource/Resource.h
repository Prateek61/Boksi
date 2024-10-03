#pragma once

#include "Boksi/Core/Resource/Buffer.h"

#include <cstdint>
#include <filesystem>

namespace Boksi
{
	// Base Resource Class for other resources to inherit from
	class Resource
	{
	public:
		Resource() = default;
		Resource(std::filesystem::path path) : m_ResourceID(0) ,m_ResourcePath(std::move(path)) {}
		virtual ~Resource() = default;

		virtual void LoadResource() = 0;
		virtual void UnloadResource() = 0;

		virtual uint64_t GetResourceSize() const = 0;
		virtual bool IsLoaded() const = 0;

		uint32_t GetResourceID() const { return m_ResourceID; }
		// Only the ResourceManager should be able to set the resource ID
		void SetResourceID(uint32_t id) { m_ResourceID = id; }

		const std::filesystem::path& GetResourcePath() const { return m_ResourcePath; }
		void SetResourcePath(std::filesystem::path path) { m_ResourcePath = std::move(path); }

	private:
		uint32_t m_ResourceID;
		std::filesystem::path m_ResourcePath;
	};


	class BinaryResource final: public Resource
	{
	public:
		BinaryResource() = default;
		BinaryResource(std::filesystem::path path) : Resource(std::move(path)), m_DataBuffer() {}
		~BinaryResource() override;

		void LoadResource() override;
		void UnloadResource() override;
		uint64_t GetResourceSize() const override { return m_DataBuffer.Size; }
		bool IsLoaded() const override { return m_DataBuffer; }

		void StoreToFile();

		Buffer& GetDataBuffer() { return m_DataBuffer; }
		Buffer StealBuffer();
		Buffer CopyBuffer() const;
		void SetDataBuffer(Buffer buffer);

	private:
		Buffer m_DataBuffer;
	};
}