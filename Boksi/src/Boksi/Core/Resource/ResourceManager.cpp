#include "bkpch.h"

#include "ResourceManager.h"

namespace
{
    Boksi::ResourceManager* s_Instance = nullptr;
    uint32_t s_ResourceID = 1;
}

namespace Boksi
{
	void ResourceManager::Init(const std::filesystem::path& basePath)
    {
        BK_PROFILE_FUNCTION();

        if (s_Instance)
        {
			BK_CORE_WARN("ResourceManager::Init: ResourceManager already initialized");
            return;
        }

        s_Instance = new ResourceManager();
    }

    void ResourceManager::Shutdown()
    {
        if ( !s_Instance )
        {
            BK_CORE_WARN("ResourceManager::Shotdown: Resource Manager not initialized");
            return;
        }

        s_Instance->UnloadAllResources();
        delete s_Instance;
    }

    ResourceManager& ResourceManager::GetInstance()
    {
		BK_CORE_ASSERT(s_Instance, "ResourceManager::GetInstance: Resource Manager not initialized");

		return *s_Instance;
    }

    const ResourceManager* ResourceManager::GetInstancePtr()
    {
        BK_CORE_ASSERT(s_Instance, "ResourceManager::GetInstance: Resource Manager not initialzied");

        return s_Instance;
    }

    void ResourceManager::AddResource(const Ref<Resource>& resource, const std::string& name, const std::filesystem::path& path)
    {
        BK_PROFILE_FUNCTION();

		resource->SetResourceID(s_ResourceID++);
        resource->SetResourcePath(m_BasePath / path);

		m_Resources.emplace(name, resource);
    }

    void ResourceManager::RemoveResource(const std::string& name)
    {
        BK_PROFILE_FUNCTION();

		auto itr = m_Resources.find(name);

		if (itr != m_Resources.end())
		{
            itr->second->UnloadResource();
			m_Resources.erase(itr);
            return;
		}

		BK_CORE_ERROR("ResourceManager::RemoveResource: Resource not found");
    }

    void ResourceManager::UnloadAllResources()
    {
        BK_PROFILE_FUNCTION();

		// Iterator over all resources and unload them
		for (auto& resource : std::views::values(m_Resources))
		{
            resource->UnloadResource();
		}
    }
}