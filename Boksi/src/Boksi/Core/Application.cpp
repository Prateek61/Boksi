#include "bkpch.h"
#include "Application.h"

#include "Boksi/Events/ApplicationEvent.h"

namespace Boksi
{
	Application::Application()
		: m_Window(Window::Create())
	{
		m_Window->SetEventCallback(BK_BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BK_BIND_EVENT_FN(OnWindowClose));

		BK_CORE_TRACE("{0}", e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}
	
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}


