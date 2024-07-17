#include <Boksi.h>

class ExampleLayer : public Boksi::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		// BK_INFO("ExampleLayer::Update");
	}

	void OnEvent(Boksi::Event& event) override
	{
		// BK_TRACE("{0}", event.ToString());
	}
};

class Sandbox : public Boksi::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Boksi::ImGuiLayer());
	}
	~Sandbox() {};
};

Boksi::Application* Boksi::CreateApplication()
{
	return new Sandbox();
}