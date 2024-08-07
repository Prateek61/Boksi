#pragma once
#include "bkpch.h"

#include "Boksi/Core/Window.h"
#include "Boksi/Core/LayerStack.h"
#include "Boksi/Events/ApplicationEvent.h"
#include "Boksi/Imgui/ImGuiLayer.h"
#include "Boksi/Core/Timestep.h"

namespace Boksi
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(Event &e);

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *overlay);

		inline Window &GetWindow();
		inline static Application &Get();

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer *m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		TimeStep m_TimeStep;
		float m_LastFrameTime = 0.0f;
	private:
		static Application *s_Instance;
	};

	// To be defined in CLIENT
	Application *CreateApplication();
}

namespace Boksi
{
	Window &Application::GetWindow()
	{
		return *m_Window;
	}

	Application &Application::Get()
	{
		return *s_Instance;
	}
}
