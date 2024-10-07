#include "Boksi.h"
#include "Boksi/Core/EntryPoint.h"

#include "SandboxLayer.h"

class Sandbox : public Boksi::Application
{
public:
	Sandbox()
	{
		Boksi::ResourceManager::Init("../Boksi/res/");

		PushLayer(new SandboxLayer());
	}

	~Sandbox() override
	{
		Boksi::ResourceManager::Shutdown();
	}
};

Boksi::Application* Boksi::CreateApplication()
{
	return new Sandbox();
}