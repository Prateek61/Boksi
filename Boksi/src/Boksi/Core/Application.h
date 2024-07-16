#pragma once
#include "bkpch.h"

#include "Boksi/Core/Window.h"
#include "Boksi/Core/LayerStack.h"
#include "Boksi/Events/ApplicationEvent.h"

namespace Boksi
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(Event& e);	

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		Scope<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& e);
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}



