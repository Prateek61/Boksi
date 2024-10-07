#include "SandboxLayer.h"

void SandboxLayer::OnAttach()
{
	BK_INFO("SandboxLayer OnAttach");

	auto& resource_manager = Boksi::ResourceManager::GetInstance();

	{
		Boksi::Ref<Boksi::BinaryResource> bin_res = Boksi::CreateRef < Boksi::BinaryResource >();
		resource_manager.AddResource(bin_res, "camera_shader", "Shaders/utils/camera.glsl");
	}

	{
		auto bin_res = resource_manager.GetResource<Boksi::BinaryResource>("camera_shader");
		auto res = bin_res.lock();
		res->LoadResource();

		if (res->IsLoaded())
		{
			BK_INFO("Camera shader:\n {0}", res->GetDataBuffer().As<char>());
		}
	}


	BK_INFO("Sandbox Layer OnAttach Complete");
}
