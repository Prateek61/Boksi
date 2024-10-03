#pragma once

#include "Boksi/Core/Resource/Resource.h"
#include "Boksi/Core/Resource/Buffer.h"

namespace Boksi
{
	class ResourceManager
	{
	public:
		static void Init(const std::filesystem::path& basePath = "");
		static void Shutdown();
		static ResourceManager& GetInstance();
		static const ResourceManager* GetInstancePtr();

		const std::filesystem::path& GetBasePath() const { return m_BasePath; }
		void SetBasePath(const std::filesystem::path& path) { m_BasePath = path; }

		void AddResource(const Ref<Resource>& resource, const std::string& name, const std::filesystem::path& path);
		void RemoveResource(const std::string& name);

		template <typename T>
		WeakRef<T> GetResource(const std::string& name);

		virtual ~ResourceManager() = default;
	private:
		ResourceManager() = default;
		void UnloadAllResources();

		std::filesystem::path m_BasePath;
		std::unordered_map<std::string, Ref<Resource>> m_Resources;
	};
}

namespace Boksi
{
	template <typename T>
	WeakRef<T> ResourceManager::GetResource(const std::string& name)
	{
		BK_PROFILE_FUNCTION();

		// Check if T is a child of Resource
		static_assert(std::is_base_of_v<Resource, T>, "T must be a child of Resource");

		auto itr = m_Resources.find(name);
		BK_CORE_ASSERT(itr != m_Resources.end(), "Resource Not Found");

		return std::static_pointer_cast<T>(itr->second);
	}
}
