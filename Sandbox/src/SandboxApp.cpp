#include <Boksi.h>
#include "imgui/imgui/imgui.h"

class ExampleLayer : public Boksi::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{

		// if (Boksi::Input::IsKeyPressed(Boksi::Key::Tab))
		// 	BK_TRACE("Tab key is pressed!");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();
	}

	void OnEvent(Boksi::Event &event) override
	{

		if (event.GetEventType() == Boksi::EventType::KeyPressed)
		{
			Boksi::KeyPressedEvent &e = (Boksi::KeyPressedEvent &)event;
			if (e.GetKeyCode() == Boksi::Key::Tab)
				BK_TRACE("Tab key is pressed (event)!");
			BK_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Boksi::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox(){};
};

Boksi::Application *Boksi::CreateApplication()
{
	return new Sandbox();
}