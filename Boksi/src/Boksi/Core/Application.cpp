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
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BK_BIND_EVENT_FN(OnWindowClose));

		BK_CORE_TRACE("{0}", e.ToString());
	}
	
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}


